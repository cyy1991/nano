#include "model.h"
#include "common/logger.h"
#include "common/timer.h"
#include "common/random.hpp"
#include "losses/loss_square.hpp"
#include "optimize/opt_lbfgs.hpp"
#include "task.h"
#include <fstream>

namespace ncv
{
        /////////////////////////////////////////////////////////////////////////////////////////

        model_t::model_t(const string_t& parameters, const string_t& description)
                :       clonable_t<model_t>(parameters, description),
                        m_rows(0),
                        m_cols(0),
                        m_outputs(0),
                        m_nparams(0),
                        m_color(color_mode::luma)
        {
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool model_t::save(const string_t& path) const
        {
                std::ofstream os(path, std::ios::binary);

                boost::archive::binary_oarchive oa(os);
                oa << m_rows;
                oa << m_cols;
                oa << m_outputs;
                oa << m_nparams;
                oa << m_color;

                return save(oa) && os.good();
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool model_t::load(const string_t& path)
        {
                std::ifstream is(path, std::ios::binary);

                boost::archive::binary_iarchive ia(is);
                ia >> m_rows;
                ia >> m_cols;
                ia >> m_outputs;
                ia >> m_nparams;
                ia >> m_color;

                return load(ia) && is.good();
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        const tensor_t& model_t::forward(const image_t& image, const rect_t& region) const
        {
                return forward(image, geom::left(region), geom::top(region));
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        const tensor_t& model_t::forward(const image_t& image, coord_t x, coord_t y) const
        {
                return forward(make_input(image, x, y));
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        const tensor_t& model_t::forward(const vector_t& input) const
        {
                assert(static_cast<size_t>(input.size()) == isize());

                tensor_t xinput(idims(), irows(), icols());
                xinput.copy_from(input.data());

                return forward(xinput);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        tensor_t model_t::make_input(const image_t& image, coord_t x, coord_t y) const
        {
                tensor_t data;

                const rect_t region = geom::make_rect(x, y, icols(), irows());

                switch (m_color)
                {
                case color_mode::luma:
                        data.resize(1, irows(), icols());
                        data.copy_plane_from(0, load_luma(image, region));
                        break;

                case color_mode::rgba:
                        data.resize(3, irows(), icols());
                        data.copy_plane_from(0, load_red(image, region));
                        data.copy_plane_from(1, load_green(image, region));
                        data.copy_plane_from(2, load_blue(image, region));
                        break;
                }

                return data;
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        tensor_t model_t::make_input(const image_t& image, const rect_t& region) const
        {
                return make_input(image, geom::left(region), geom::top(region));
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        size_t model_t::idims() const
        {
                switch (m_color)
                {
                case color_mode::rgba:
                        return 3;

                case color_mode::luma:
                default:
                        return 1;
                }
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool model_t::resize(const task_t& task, bool verbose)
        {
                return resize(task.n_rows(), task.n_cols(), task.n_outputs(), task.color(), verbose);
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        bool model_t::resize(size_t rows, size_t cols, size_t outputs, color_mode color, bool verbose)
        {
                m_rows = rows;
                m_cols = cols;
                m_outputs = outputs;
                m_color = color;
                m_nparams = resize(verbose);

                if (verbose)
                {
                        log_info() << "model: parameters = " << psize() << ".";
                }

                return true;
        }

        /////////////////////////////////////////////////////////////////////////////////////////

        tensor_t model_t::generate(const vector_t& target) const
        {
                const square_loss_t loss;

                // construct the optimization problem
                const timer_t timer;

                auto fn_size = [&] ()
                {
                        return isize();
                };

                auto fn_fval = [&] (const vector_t& x)
                {
                        const tensor_t output = this->forward(x);

                        return loss.value(target, output.vector());
                };

                auto fn_fval_grad = [&] (const vector_t& x, vector_t& gx)
                {
                        const tensor_t output = this->forward(x);
                        const vector_t ograd = loss.vgrad(target, output.vector());

                        gx = this->backward(ograd).vector();

                        return loss.value(target, output.vector());
                };

                auto fn_wlog = [] (const string_t& message)
                {
                        log_warning() << message;
                };
                auto fn_elog = [] (const string_t& message)
                {
                        log_error() << message;
                };
                auto fn_ulog = [&] (const opt_state_t& /*result*/, const timer_t& /*timer*/)
                {
//                        log_info() << "[loss = " << result.f
//                                   << ", grad = " << result.g.lpNorm<Eigen::Infinity>()
//                                   << ", funs = " << result.n_fval_calls() << "/" << result.n_grad_calls()
//                                   << "] done in " << timer.elapsed() << ".";
                };

                // assembly optimization problem & optimize the input
                const opt_problem_t problem(fn_size, fn_fval, fn_fval_grad);

                const opt_opulog_t fn_ulog_ref = std::bind(fn_ulog, _1, std::ref(timer));

                const size_t iterations = 256;
                const scalar_t epsilon = 1e-6;

                tensor_t input(idims(), irows(), icols());
                input.random(random_t<scalar_t>(0.0, 1.0));

                const opt_state_t result = optimize::lbfgs(problem, input.vector(), iterations, epsilon,
                                                           fn_wlog, fn_elog, fn_ulog_ref);
                input.copy_from(result.x.data());

                log_info() << "[loss = " << result.f
                           << ", grad = " << result.g.lpNorm<Eigen::Infinity>()
                           << ", funs = " << result.n_fval_calls() << "/" << result.n_grad_calls()
                           << "] done in " << timer.elapsed() << ".";

                // OK
                return input;
        }

        /////////////////////////////////////////////////////////////////////////////////////////
}
