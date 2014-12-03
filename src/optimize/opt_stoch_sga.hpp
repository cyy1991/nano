#pragma once

#include "opt_stoch.hpp"
#include <cassert>

namespace ncv
{
        namespace optimize
        {
                ///
                /// \brief stochastic gradient average gradient (descent) starting from the initial value (guess) x0
                ///
                /// NB: "Minimizing Finite Sums with the Stochastic Average Gradient"
                ///     - Mark Schmidth, Nicolas Le Roux, Francis Bach
                ///
                template
                <
                        // learning rate's decay rate
                        decay_rate tbeta,

                        // optimization problem
                        typename tproblem,

                        // dependent types
                        typename tscalar = typename tproblem::tscalar,
                        typename tsize = typename tproblem::tsize,
                        typename tvector = typename tproblem::tvector,
                        typename tstate = typename tproblem::tstate,
                        typename tulog = typename tproblem::tulog
                >
                tstate stoch_sga(
                        const tproblem& problem,
                        const tvector& x0,
                        tsize epochs,                           ///< number of epochs
                        tsize iterations,                       ///< epoch size in number of iterations
                        tscalar alpha0,                         ///< initial learning rate
                        const tulog& op_ulog = tulog())         ///< called after each epoch with the current state
                {
                        assert(problem.size() == static_cast<tsize>(x0.size()));

                        tstate cstate(problem, x0);     // current state

                        tvector gavg = x0;              // running-averaged gradient
                        gavg.setZero();

                        tscalar alpha = alpha0;         // learning rate
                        tscalar sumb = tscalar(1) / alpha;

                        for (tsize e = 0, k = 0; e < epochs; e ++)
                        {
                                for (tsize i = 0; i < iterations; i ++, alpha = optimize::decay(alpha0, k ++, tbeta))
                                {
                                        // descent direction
                                        const tscalar b = tscalar(1) / alpha;
                                        gavg = (gavg * sumb + cstate.g * b) / (sumb + b);
                                        sumb = sumb + b;

                                        cstate.d = -gavg;

                                        // update solution
                                        cstate.update(problem, alpha);
                                }

                                if (op_ulog)
                                {
                                        op_ulog(cstate);
                                }
                        }

                        return cstate;
                }
        }
}

