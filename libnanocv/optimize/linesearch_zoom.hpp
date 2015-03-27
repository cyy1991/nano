#pragma once

#include <algorithm>
#include "linesearch.h"
#include "linesearch_step.hpp"
#include "linesearch_cubic.hpp"

namespace ncv
{
        namespace optimize
        {
                template
                <
                        typename tproblem,

                        // dependent types
                        typename tscalar = typename tproblem::tscalar,
                        typename tsize = typename tproblem::tsize,
                        typename tvector = typename tproblem::tvector,
                        typename tstate = typename tproblem::tstate
                >
                tscalar ls_zoom(const tproblem& problem, const ls_step_t<tproblem>& step0,
                        const ls_strategy strategy, const tscalar c1, const tscalar c2,
                        ls_step_t<tproblem> steplo,
                        ls_step_t<tproblem> stephi,
                        ls_step_t<tproblem>& stept,
                        const tsize max_iters = 64)
                {
                        // (Nocedal & Wright (numerical optimization 2nd) @ p.60)
                        for (size_t i = 1; i <= max_iters; i ++)
                        {
                                tscalar t;

                                switch (strategy)
                                {
                                case ls_strategy::interpolation_cubic:
                                        {
                                                // cubic interpolation (if feasible)
                                                const tscalar tmin = std::min(steplo.alpha(), stephi.alpha());
                                                const tscalar tmax = std::max(steplo.alpha(), stephi.alpha());
                                                const tscalar teps = (tmax - tmin) / 100;

                                                const tscalar tc = ls_cubic(steplo, stephi);
                                                if (tmin + teps < tc && tc < tmax - teps)
                                                {
                                                        t = tc;
                                                        break;
                                                }
                                        }
                                        // fallthrough!

                                case ls_strategy::interpolation_bisection:
                                default:
                                        {
                                                // bisection
                                                t = (steplo.alpha() + stephi.alpha()) / 2;
                                        }
                                        break;
                                }

                                // check sufficient decrease
                                if (!stept.reset_with_grad(t))
                                {
                                        // poorly scaled problem?!
                                        return 0.0;
                                }

                                if (!stept.has_armijo(step0, c1) || stept.func() >= steplo.func())
                                {
                                        stephi = stept;
                                }

                                // check curvature
                                else
                                {
                                        if (stept.has_strong_wolfe(step0, c2))
                                        {
                                                return stept.setup();
                                        }

                                        if (stept.gphi() * (stephi.alpha() - steplo.alpha()) >= 0)
                                        {
                                                stephi = steplo;
                                        }

                                        steplo = stept;
                                }
                        }

                        // OK, give up
                        return 0;
                }
        }
}

