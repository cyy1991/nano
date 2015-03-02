#pragma once

#include "batch_params.hpp"
#include "linesearch_init_interp.hpp"
#include "linesearch_wolfe.hpp"
#include <cassert>

namespace ncv
{
        namespace optimize
        {
                ///
                /// \brief gradient descent
                ///
                template
                <
                        typename tproblem                       ///< optimization problem
                >
                struct batch_gd_t : public batch_params_t<tproblem>
                {
                        typedef batch_params_t<tproblem>        base_t;

                        typedef typename base_t::tscalar        tscalar;
                        typedef typename base_t::tsize          tsize;
                        typedef typename base_t::tvector        tvector;
                        typedef typename base_t::tstate         tstate;
                        typedef typename base_t::twlog          twlog;
                        typedef typename base_t::telog          telog;
                        typedef typename base_t::tulog          tulog;

                        ///
                        /// \brief constructor
                        ///
                        batch_gd_t(     tsize max_iterations,
                                        tscalar epsilon,
                                        const twlog& wlog = twlog(),
                                        const telog& elog = telog(),
                                        const tulog& ulog = tulog())
                                :       base_t(max_iterations, epsilon, wlog, elog, ulog)
                        {
                        }

                        ///
                        /// \brief minimize starting from the initial guess x0
                        ///
                        tstate operator()(const tproblem& problem, const tvector& x0) const
                        {
                                assert(problem.size() == static_cast<tsize>(x0.size()));

                                tstate cstate(problem, x0);             // current state

                                // line-search initial step length
                                linesearch_init_interpolation_t<tstate> ls_init;

                                // line-search step
                                linesearch_wolfe_t<tproblem> ls_step(1e-4, 0.1);

                                // iterate until convergence
                                for (tsize i = 0; i < base_t::m_max_iterations; i ++)
                                {
                                        base_t::ulog(cstate);

                                        // check convergence
                                        if (cstate.converged(base_t::m_epsilon))
                                        {
                                                break;
                                        }

                                        // descent direction
                                        cstate.d = -cstate.g;

                                        // line-search
                                        const tscalar t0 = ls_init.update(cstate);
                                        if (!ls_step.update(problem, t0, cstate))
                                        {
                                                base_t::elog("line-search failed for GD!");
                                                break;
                                        }
                                }

                                return cstate;
                        }
                };
        }
}

