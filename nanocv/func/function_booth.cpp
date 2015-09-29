#include "function_booth.h"

namespace ncv
{
        struct function_booth_t : public function_t
        {
                virtual string_t name() const override
                {
                        return "Booth";
                }

                virtual opt_problem_t problem() const override
                {
                        const opt_opsize_t fn_size = [=] ()
                        {
                                return 2;
                        };

                        const opt_opfval_t fn_fval = [=] (const opt_vector_t& x)
                        {
                                const opt_scalar_t a = x(0), b = x(1);

                                const opt_scalar_t u = a + 2 * b - 7;
                                const opt_scalar_t v = 2 * a + b - 5;

                                return u * u + v * v;
                        };

                        const opt_opgrad_t fn_grad = [=] (const opt_vector_t& x, opt_vector_t& gx)
                        {
                                const opt_scalar_t a = x(0), b = x(1);

                                const opt_scalar_t u = a + 2 * b - 7;
                                const opt_scalar_t v = 2 * a + b - 5;

                                gx.resize(2);
                                gx(0) = 2 * u + 2 * v * 2;
                                gx(1) = 2 * u * 2 + 2 * v;

                                return fn_fval(x);
                        };

                        return opt_problem_t(fn_size, fn_fval, fn_grad);
                }

                virtual bool is_valid(const opt_vector_t& x) const override
                {
                        return norm(x) < 10.0;
                }

                virtual bool is_minima(const opt_vector_t& x, const opt_scalar_t epsilon) const override
                {
                        const auto xmins =
                        {
                                std::vector<opt_scalar_t>{ 1.0, 3.0 }
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

        functions_t make_booth_funcs()
        {
                return { std::make_shared<function_booth_t>() };
        }
}
	
