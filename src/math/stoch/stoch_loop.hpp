#pragma once

#include "params.hpp"
#include "math/tune.hpp"
#include "math/momentum.hpp"

namespace nano
{
        ///
        /// \brief hyper-parameter tuning for stochastic optimizers.
        ///
        inline auto make_alpha0s()
        {
                return nano::make_finite_space(1e-3, 1e-2, 1e-1);
        }

        inline auto make_decays()
        {
                return nano::make_log10_space(-3.0, -1.0, 0.2);
        }

        inline auto make_momenta()
        {
                return nano::make_log10_space(std::log10(0.1), std::log10(0.99), 0.2);
        }

        inline auto make_epsilons()
        {
                return nano::make_finite_space(1e-4, 1e-6, 1e-8);
        }

        ///
        /// \brief stochastic optimization loop.
        ///
        template
        <
                typename tproblem,      ///< optimization problem
                typename toperator,     ///< operator to call for each optimization iteration
                typename tstate = typename stoch_params_t<tproblem>::tstate,
                typename tconfig = typename stoch_params_t<tproblem>::tconfig
        >
        auto stoch_loop(
                const tproblem& problem,
                const stoch_params_t<tproblem>& params,
                const tstate& istate,
                const toperator& op,
                const tconfig& config = tconfig())
        {
                // current state
                auto cstate = istate;

                // average state
                // - similar to average stochastic gradient descent, but using an exponential moving average
                auto astate = istate;

                const typename tproblem::tscalar momentum = 0.95;
                momentum_vector_t<typename tproblem::tvector> xavg(momentum, istate.x.size());

                // best state
                auto bstate = istate;
                bstate.f = std::numeric_limits<typename tproblem::tscalar>::max();

                // for each epoch ...
                for (std::size_t e = 0, k = 1; e < params.m_epochs; ++ e)
                {
                        // for each iteration ...
                        for (std::size_t i = 0; i < params.m_epoch_size; ++ i, ++ k)
                        {
                                op(cstate, k);
                                xavg.update(cstate.x);
                        }

                        // log the current state & check the stopping criteria
                        astate.update(problem, xavg.value());
                        if (params.tuning())
                        {
                                astate.f = params.tlog(astate, config);
                        }
                        else if (!params.ulog(astate, config))
                        {
                                break;
                        }

                        // update the best state
                        bstate.update(astate);
                }

                // OK
                return bstate;
        }
}

