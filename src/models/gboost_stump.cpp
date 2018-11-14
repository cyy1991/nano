#include "solver.h"
#include "core/tpool.h"
#include "core/logger.h"
#include "gboost_stump.h"
#include "core/ibstream.h"
#include "core/obstream.h"
#include "gboost_lsearch.h"

using namespace nano;

static auto measure(const task_t& task, const fold_t& fold, const tensor4d_t& outputs, const loss_t& loss)
{
        const auto& tpool = tpool_t::instance();

        std::vector<stats_t> errors(tpool.workers());
        std::vector<stats_t> values(tpool.workers());

        loopit(task.size(fold), [&] (const size_t i, const size_t t)
        {
                assert(t < tpool.workers());
                const auto input = task.input(fold, i);
                const auto target = task.target(fold, i);
                const auto output = outputs.tensor(i);

                errors[t](loss.error(target, output));
                values[t](loss.value(target, output));
        });

        for (size_t t = 1; t < tpool.workers(); ++ t)
        {
                errors[0](errors[t]);
                values[0](values[t]);
        }

        return std::make_pair(errors[0], values[0]);
}

static void update(const task_t& task, const fold_t& fold, tensor4d_t& outputs, const stump_t& stump)
{
        loopi(task.size(fold), [&] (const size_t i)
        {
                const auto input = task.input(fold, i);
                const auto oindex = input(stump.m_feature) < stump.m_threshold ? 0 : 1;
                outputs.array(i) += stump.m_outputs.array(oindex);
        });
}

static void fit(const task_t& task, const tensor4d_t& residuals,
        const tensor_size_t feature, const scalars_t& fvalues, const scalars_t& thresholds,
        const stump_type type, scalar_t& value, stump_t& stump)
{
        tensor3d_t residuals_pos1(task.odims()), residuals_pos2(task.odims());
        tensor3d_t residuals_neg1(task.odims()), residuals_neg2(task.odims());

        for (size_t t = 0; t + 1 < thresholds.size(); ++ t)
        {
                const auto threshold = (thresholds[t + 0] + thresholds[t + 1]) / 2;

                residuals_pos1.zero(), residuals_pos2.zero();
                residuals_neg1.zero(), residuals_neg2.zero();

                int cnt_pos = 0, cnt_neg = 0;
                for (size_t i = 0, size = fvalues.size(); i < size; ++ i)
                {
                        const auto residual = residuals.array(i);
                        if (fvalues[i] < threshold)
                        {
                                cnt_neg ++;
                                residuals_neg1.array() += residual;
                                residuals_neg2.array() += residual * residual;
                        }
                        else
                        {
                                cnt_pos ++;
                                residuals_pos1.array() += residual;
                                residuals_pos2.array() += residual * residual;
                        }
                }

                const auto tvalue =
                        (residuals_pos2.array() - residuals_pos1.array().square() / cnt_pos).sum() +
                        (residuals_neg2.array() - residuals_neg1.array().square() / cnt_neg).sum();

                if (tvalue < value)
                {
                        value = tvalue;
                        stump.m_feature = feature;
                        stump.m_threshold = threshold;
                        stump.m_outputs.resize(cat_dims(2, task.odims()));
                        switch (type)
                        {
                        case stump_type::discrete:
                                stump.m_outputs.vector(0) = residuals_neg1.array().sign();
                                stump.m_outputs.vector(1) = residuals_pos1.array().sign();
                                break;

                        default:
                                stump.m_outputs.vector(0) = residuals_neg1.vector() / cnt_neg;
                                stump.m_outputs.vector(1) = residuals_pos1.vector() / cnt_pos;
                                break;
                        }
                }
        }
}

void gboost_stump_t::to_json(json_t& json) const
{
        nano::to_json(json,
                "rounds", m_rounds,
                "patience", m_patience,
                "stump", m_stump_type, "stumps", join(enum_values<stump_type>()),
                "solver", m_solver, "solvers", join(get_solvers().ids()),
                "tune", m_gboost_tune, "tunes", join(enum_values<gboost_tune>()));
}

void gboost_stump_t::from_json(const json_t& json)
{
        nano::from_json(json,
                "rounds", m_rounds,
                "patience", m_patience,
                "stump", m_stump_type,
                "solver", m_solver,
                "tune", m_gboost_tune);
}

trainer_result_t gboost_stump_t::train(const task_t& task, const size_t fold, const loss_t& loss)
{
        // check if the solver is properly set
        rsolver_t solver;
        critical(solver = get_solvers().get(m_solver),
                strcat("search solver (", m_solver, ")"));

        m_idims = task.idims();
        m_odims = task.odims();

        m_stumps.clear();

        trainer_result_t result("<config>");
        timer_t timer;

        const auto fold_tr = fold_t{fold, protocol::train};
        const auto fold_vd = fold_t{fold, protocol::valid};
        const auto fold_te = fold_t{fold, protocol::test};

        tensor4d_t outputs_tr(cat_dims(task.size(fold_tr), m_odims));
        tensor4d_t outputs_vd(cat_dims(task.size(fold_vd), m_odims));
        tensor4d_t outputs_te(cat_dims(task.size(fold_te), m_odims));

        outputs_tr.zero();
        outputs_vd.zero();
        outputs_te.zero();

        stats_t errors_tr, errors_vd, errors_te;
        stats_t values_tr, values_vd, values_te;

        std::tie(errors_tr, values_tr) = ::measure(task, fold_tr, outputs_tr, loss);
        std::tie(errors_vd, values_vd) = ::measure(task, fold_vd, outputs_vd, loss);
        std::tie(errors_te, values_te) = ::measure(task, fold_te, outputs_te, loss);

        const auto measure_tr = trainer_measurement_t{values_tr.avg(), errors_tr.avg()};
        const auto measure_vd = trainer_measurement_t{values_vd.avg(), errors_vd.avg()};
        const auto measure_te = trainer_measurement_t{values_te.avg(), errors_te.avg()};

        const auto status = result.update(
                trainer_state_t{timer.milliseconds(), 0, measure_tr, measure_vd, measure_te},
                m_patience);

        log_info() << "[" << 0 << "/" << m_rounds
                << "]:tr=" << measure_tr
                << ",vd=" << measure_vd << "|" << status
                << ",te=" << measure_te << ".";

        stump_t stump;
        tensor4d_t residuals_tr(cat_dims(task.size(fold_tr), m_odims));
        tensor4d_t stump_outputs_tr(cat_dims(task.size(fold_tr), m_odims));

        tensor4d_t targets(cat_dims(task.size(fold_tr), m_odims));
        for (size_t i = 0, size = task.size(fold_tr); i < size; ++ i)
        {
                const auto target = task.target(fold_tr, i);
                targets.tensor(i) = target;
        }

        gboost_lsearch_function_t func(targets, outputs_tr, stump_outputs_tr, loss);

        for (auto round = 0; round < m_rounds && !result.is_done(); ++ round)
        {
                for (size_t i = 0, size = task.size(fold_tr); i < size; ++ i)
                {
                        const auto input = task.input(fold_tr, i);
                        const auto target = task.target(fold_tr, i);
                        const auto output = outputs_tr.tensor(i);

                        const auto vgrad = loss.vgrad(target, output);
                        residuals_tr.vector(i) = -vgrad.vector();
                }

                // todo: generalize this - e.g. to use features that are products of two input features, use LBPs|HOGs
                scalar_t value = std::numeric_limits<scalar_t>::max();
                for (auto feature = 0; feature < nano::size(m_idims); ++ feature)
                {
                        scalars_t fvalues(task.size(fold_tr));
                        for (size_t i = 0, size = task.size(fold_tr); i < size; ++ i)
                        {
                                const auto input = task.input(fold_tr, i);
                                fvalues[i] = input(feature);
                        }

                        auto thresholds = fvalues;
                        std::sort(thresholds.begin(), thresholds.end());
                        thresholds.erase(
                                std::unique(thresholds.begin(), thresholds.end()),
                                thresholds.end());

                        fit(task, residuals_tr, feature, fvalues, thresholds, m_stump_type, value, stump);
                }

                // line-search
                for (size_t i = 0, size = task.size(fold_tr); i < size; ++ i)
                {
                        const auto input = task.input(fold_tr, i);
                        const auto oindex = input(stump.m_feature) < stump.m_threshold ? 0 : 1;
                        stump_outputs_tr.tensor(i) = stump.m_outputs.tensor(oindex);
                }

                const auto state = solver->minimize(100, epsilon2<scalar_t>(), func, vector_t::Constant(1, 0));
                const auto step = state.x(0);

                // todo: break if the line-search fails (e.g. if state.m_iterations == 0

                stump.m_outputs.vector() *= step;
                m_stumps.push_back(stump);

                // update current outputs
                update(task, fold_tr, outputs_tr, stump);
                update(task, fold_vd, outputs_vd, stump);
                update(task, fold_te, outputs_te, stump);

                std::tie(errors_tr, values_tr) = ::measure(task, fold_tr, outputs_tr, loss);
                std::tie(errors_vd, values_vd) = ::measure(task, fold_vd, outputs_vd, loss);
                std::tie(errors_te, values_te) = ::measure(task, fold_te, outputs_te, loss);

                const auto measure_tr = trainer_measurement_t{values_tr.avg(), errors_tr.avg()};
                const auto measure_vd = trainer_measurement_t{values_vd.avg(), errors_vd.avg()};
                const auto measure_te = trainer_measurement_t{values_te.avg(), errors_te.avg()};

                const auto status = result.update(
                        trainer_state_t{timer.milliseconds(), round + 1, measure_tr, measure_vd, measure_te},
                        m_patience);

                log_info() << "[" << (round + 1) << "/" << m_rounds
                        << "]:tr=" << measure_tr
                        << ",vd=" << measure_vd << "|" << status
                        << ",te=" << measure_te
                        << ",stump=(f=" << stump.m_feature << ",t=" << stump.m_threshold << ")"
                        << ",solver=(" << state.m_status << ",i=" << state.m_iterations
                        << ",x=" << state.x(0) << ",f=" << state.f << ",g=" << state.convergence_criteria() << ").";
        }

        // keep only the stumps up to optimum epoch (on the validation dataset)
        m_stumps.erase(
                m_stumps.begin() + result.optimum().m_epoch,
                m_stumps.end());

        return result;
}

tensor3d_t gboost_stump_t::output(const tensor3d_t& input) const
{
        assert(input.dims() == m_idims);

        tensor3d_t output(m_odims);
        output.zero();

        const auto idata = input.array();
        auto odata = output.array();

        for (const auto& stump : m_stumps)
        {
                const auto oindex = idata(stump.m_feature) < stump.m_threshold ? 0 : 1;
                odata.array() += stump.m_outputs.array(oindex);
        }

        return output;
}

bool gboost_stump_t::save(obstream_t& stream) const
{
        if (    !stream.write(m_idims) ||
                !stream.write(m_odims) ||
                !stream.write(m_rounds) ||
                !stream.write(m_stump_type) ||
                !stream.write(m_gboost_tune) ||
                !stream.write(m_stumps.size()))
        {
                return false;
        }

        for (const auto& stump : m_stumps)
        {
                assert(stump.m_feature >= 0 && stump.m_feature < nano::size(m_idims));
                assert(stump.m_outputs.dims() == cat_dims(2, m_odims));

                if (    !stream.write(stump.m_feature) ||
                        !stream.write(stump.m_threshold) ||
                        !stream.write_tensor(stump.m_outputs))
                {
                       return false;
                }
        }

        return true;
}

bool gboost_stump_t::load(ibstream_t& stream)
{
        size_t n_stumps = 0;
        if (    !stream.read(m_idims) ||
                !stream.read(m_odims) ||
                !stream.read(m_rounds) ||
                !stream.read(m_stump_type) ||
                !stream.read(m_gboost_tune) ||
                !stream.read(n_stumps))
        {
                return false;
        }

        m_stumps.resize(n_stumps);
        for (auto& stump : m_stumps)
        {
                if (    !stream.read(stump.m_feature) ||
                        !stream.read(stump.m_threshold) ||
                        !stream.read_tensor(stump.m_outputs) ||
                        stump.m_feature < 0 ||
                        stump.m_feature >= nano::size(m_idims) ||
                        stump.m_outputs.dims() != cat_dims(2, m_odims))
                {
                        return false;
                }
        }

        // todo: more verbose loading (#stumps, feature or coefficient statistics, idims...)

        return true;
}

probes_t gboost_stump_t::probes() const
{
        // todo: add probes here to measure the training and the evaluation time
        probes_t probes;
        return probes;
}