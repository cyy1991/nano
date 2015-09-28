#include "function_beale.h"

namespace ncv
{
        struct function_beale_t : public function_t
        {
                virtual string_t name() const override
                {
                        return "Beale";
                }

                virtual opt_problem_t problem() const override
                {
                        const opt_opsize_t fn_size = [=] ()
                        {
                                return 2;
                        };

                        const opt_opfval_t fn_fval = [=] (const vector_t& x)
                        {
                                const long double a = x(0);
                                const long double b = x(1), b2 = b * b, b3 = b2 * b;

                                const long double z0 = 1.5 - a + a * b;
                                const long double z1 = 2.25 - a + a * b2;
                                const long double z2 = 2.625 - a + a * b3;

                                return static_cast<scalar_t>(z0 * z0 + z1 * z1 + z2 * z2);
                        };

                        const opt_opgrad_t fn_grad = [=] (const vector_t& x, vector_t& gx)
                        {
                                const long double a = x(0);
                                const long double b = x(1), b2 = b * b, b3 = b2 * b;

                                const long double z0 = 1.5 - a + a * b;
                                const long double z1 = 2.25 - a + a * b2;
                                const long double z2 = 2.625 - a + a * b3;

                                gx.resize(2);
                                gx(0) = static_cast<scalar_t>(2.0 * (z0 * (-1 + b) + z1 * (-1 + b2) + z2 * (-1 + b3)));
                                gx(1) = static_cast<scalar_t>(2.0 * (z0 * (a) + z1 * (2 * a * b) + z2 * (3 * a * b2)));

                                return fn_fval(x);
                        };

                        return opt_problem_t(fn_size, fn_fval, fn_grad);
                }

                virtual bool is_valid(const vector_t& x) const override
                {
                        return norm(x) < 4.5;
                }

                virtual bool is_minima(const vector_t& x, const scalar_t epsilon) const override
                {
                        const auto xmins =
                        {
                                scalars_t{ 3.0, 0.5 }
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

        functions_t make_beale_funcs()
        {
                return { std::make_shared<function_beale_t>() };
        }
}
	
