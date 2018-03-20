#include "math/epsilon.h"
#include "solver_stoch_cocob.h"

using namespace nano;

tuner_t stoch_cocob_t::configs() const
{
        tuner_t tuner;
        tuner.add("alpha", make_pow10_scalars(0, +1, +3)).precision(0);
        tuner.add("epsilon", make_pow10_scalars(0, -7, -2)).precision(7);
        return tuner;
}

void stoch_cocob_t::from_json(const json_t& json)
{
        nano::from_json(json, "alpha", m_alpha, "epsilon", m_epsilon);
}

void stoch_cocob_t::to_json(json_t& json) const
{
        nano::to_json(json, "alpha", m_alpha, "epsilon", m_epsilon);
}

solver_state_t stoch_cocob_t::minimize(const stoch_params_t& param, const function_t& function, const vector_t& x0) const
{
        vector_t L = vector_t::Constant(x0.size(), m_epsilon);
        vector_t G = vector_t::Zero(x0.size());
        vector_t theta = vector_t::Zero(x0.size());
        vector_t reward = vector_t::Zero(x0.size());

        const auto solver = [&] (solver_state_t& cstate, const solver_state_t&)
        {
                // descent direction
                L = L.array().max(cstate.g.array().abs());
                G = G.array() + cstate.g.array().abs();
                theta = theta.array() - cstate.g.array();
                reward = (reward.array() - (cstate.x - x0).array() * cstate.g.array()).max(0);

                cstate.x = x0.array() +
                        theta.array() * (L + reward).array() /
                        (L.array() * (G + L).array().max(m_alpha * L.array()));

                // update solution
                function.stoch_next();
                cstate.stoch_update(function, cstate.x);
        };

        const auto snapshot = [&] (const solver_state_t& cstate, solver_state_t& sstate)
        {
                sstate.update(function, cstate.x);
        };

        return loop(param, function, x0, solver, snapshot);
}