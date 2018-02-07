#include "lrate.h"
#include "math/numeric.h"
#include "tensor/momentum.h"
#include "solver_stoch_adam.h"

using namespace nano;

tuner_t stoch_adam_t::configs() const
{
        tuner_t tuner;
        tuner.add_base10("alpha0", -4, -1);
        tuner.add_linear("decay", 0, 1);
        tuner.add_base10("epsilon", -6, -2);
        tuner.add_linear("beta1", 0.1, 0.9);
        tuner.add_base10("beta2", -6.0, -0.1, 0.9);
        return tuner;
}

json_reader_t& stoch_adam_t::config(json_reader_t& reader)
{
        return reader.object("alpha0", m_alpha0, "decay", m_decay, "epsilon", m_epsilon, "beta1", m_beta1, "beta2", m_beta2);
}

json_writer_t& stoch_adam_t::config(json_writer_t& writer) const
{
        return writer.object("alpha0", m_alpha0, "decay", m_decay, "epsilon", m_epsilon, "beta1", m_beta1, "beta2", m_beta2);
}

solver_state_t stoch_adam_t::minimize(const stoch_params_t& param, const function_t& function, const vector_t& x0) const
{
        // learning rate schedule
        lrate_t lrate(m_alpha0, m_decay, param.m_epoch_size);

        // first-order momentum of the gradient
        momentum_t<vector_t> m(m_beta1, x0.size());

        // second-order momentum of the gradient
        momentum_t<vector_t> v(m_beta2, x0.size());

        // assembly the solver
        const auto solver = [&] (solver_state_t& cstate, const solver_state_t&)
        {
                // learning rate
                const scalar_t alpha = lrate.get();

                // descent direction
                m.update(cstate.g);
                v.update(cstate.g.array().square());

                cstate.d = -m.value().array() / (m_epsilon + v.value().array().sqrt());

                // update solution
                function.stoch_next();
                cstate.stoch_update(function, alpha);
        };

        const auto snapshot = [&] (const solver_state_t& cstate, solver_state_t& sstate)
        {
                sstate.update(function, cstate.x);
        };

        return loop(param, function, x0, solver, snapshot);
}
