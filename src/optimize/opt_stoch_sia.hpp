#pragma once

#include <cassert>

namespace ncv
{
        namespace optimize
        {
                ///
                /// \brief stochastic iterative average gradient (descent) starting from the initial value (guess) x0
                ///
                /// NB: "Minimizing Finite Sums with the Stochastic Average Gradient"
                ///     - Mark Schmidth, Nicolas Le Roux, Francis Bach
                ///
                template
                <
                        typename tproblem,

                        // dependent types
                        typename tscalar = typename tproblem::tscalar,
                        typename tsize = typename tproblem::tsize,
                        typename tvector = typename tproblem::tvector,
                        typename tstate = typename tproblem::tstate,

                        typename twlog = typename tproblem::twlog,
                        typename telog = typename tproblem::telog,
                        typename tulog = typename tproblem::tulog
                >
                tstate stoch_sia(
                        const tproblem& problem,
                        const tvector& x0,
                        tsize epochs,                   ///< number of epochs
                        tsize iterations,               ///< epoch size in number of iterations
                        tscalar alpha0,                 ///< initial learning rate
                        tscalar beta,                   ///< decreasing factor for the learning rate (<1)
                        const tulog& op_ulog = tulog()) ///< called after each epoch with the current state
                {
                        assert(problem.size() == static_cast<tsize>(x0.size()));

                        tstate cstate(problem, x0);     // current state

                        tvector xavg = x0;              // running-averaged parameters
                        xavg.setZero();

                        tscalar alpha = alpha0;         // learning rate
                        tscalar sumb = tscalar(1) / alpha;

                        for (tsize e = 0; e < epochs; e ++)
                        {
                                for (tsize i = 0; i < iterations; i ++, alpha *= beta)
                                {
                                        // descent direction
                                        cstate.d = -cstate.g;

                                        // update solution
                                        cstate.update(problem, alpha);

                                        const tscalar b = tscalar(1) / alpha;
                                        xavg = (xavg * sumb + cstate.x * b) / (sumb + b);
                                        sumb = sumb + b;
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

