#pragma once

#include "function.hpp"
#include "tensor/vector.hpp"

namespace func
{
        ///
        /// \brief create Booth test functions
        ///
        /// https://en.wikipedia.org/wiki/Test_functions_for_optimization
        ///
        template
        <
                typename tscalar
        >
        struct function_booth_t : public function_t<tscalar>
        {
                typedef typename function_t<tscalar>::tsize     tsize;
                typedef typename function_t<tscalar>::tvector   tvector;
                typedef typename function_t<tscalar>::tproblem  tproblem;
                
                virtual std::string name() const override
                {
                        return "Booth";
                }

                virtual tproblem problem() const override
                {
                        const auto fn_size = [=] ()
                        {
                                return 2;
                        };

                        const auto fn_fval = [=] (const tvector& x)
                        {
                                const tscalar a = x(0), b = x(1);

                                const tscalar u = a + 2 * b - 7;
                                const tscalar v = 2 * a + b - 5;

                                return u * u + v * v;
                        };

                        const auto fn_grad = [=] (const tvector& x, tvector& gx)
                        {
                                const tscalar a = x(0), b = x(1);

                                const tscalar u = a + 2 * b - 7;
                                const tscalar v = 2 * a + b - 5;

                                gx.resize(2);
                                gx(0) = 2 * u + 2 * v * 2;
                                gx(1) = 2 * u * 2 + 2 * v;

                                return fn_fval(x);
                        };

                        return tproblem(fn_size, fn_fval, fn_grad);
                }

                virtual bool is_valid(const tvector& x) const override
                {
                        return norm(x) < 10.0;
                }

                virtual bool is_minima(const tvector& x, const tscalar epsilon) const override
                {
                        const auto xmins =
                        {
                                std::vector<tscalar>{ 1.0, 3.0 }
                        };

                        for (const auto& xmin : xmins)
                        {
                                if (distance(x, tensor::map_vector(xmin.data(), 2)) < epsilon)
                                {
                                        return true;
                                }
                        }

                        return false;
                }
        };
}
