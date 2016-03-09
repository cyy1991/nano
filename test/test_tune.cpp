#include "unit_test.hpp"
#include "math/tune.hpp"
#include "math/random.hpp"
#include "math/epsilon.hpp"

template
<
        typename tscalar
>
void check(const tscalar a, const tscalar b,
        const tscalar minlog, const tscalar maxlog, const tscalar eps)
{
        const auto log10_space = zob::make_log10_space(minlog, maxlog, eps);

        const auto min = std::pow(tscalar(10), minlog);
        const auto max = std::pow(tscalar(10), maxlog);
        const auto linear_space = zob::make_linear_space(min, max, eps);

        const auto epsilon = zob::epsilon2<tscalar>();

        const auto op1 = [a=a, b=b] (const tscalar x)
        {
                return (x - a) * (x - a) + b;
        };
        {
                const auto ret = zob::tune(op1, log10_space);

                ZOB_CHECK_CLOSE(ret.optimum(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param0(), a, epsilon);
        }
        {
                const auto ret = zob::tune(op1, linear_space);

                ZOB_CHECK_CLOSE(ret.optimum(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param0(), a, epsilon);
        }

        const auto op2 = [a=a, b=b] (const tscalar x, const tscalar y)
        {
                return (x - b) * (x - b) + (y - a) * (y - a) + b;
        };
        {
                const auto ret = zob::tune(op2, log10_space, log10_space);

                ZOB_CHECK_CLOSE(ret.optimum(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param0(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param1(), a, epsilon);
        }
        {
                const auto ret = zob::tune(op2, linear_space, log10_space);

                ZOB_CHECK_CLOSE(ret.optimum(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param0(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param1(), a, epsilon);
        }
        {
                const auto ret = zob::tune(op2, log10_space, linear_space);

                ZOB_CHECK_CLOSE(ret.optimum(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param0(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param1(), a, epsilon);
        }
        {
                const auto ret = zob::tune(op2, linear_space, linear_space);

                ZOB_CHECK_CLOSE(ret.optimum(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param0(), b, epsilon);
                ZOB_CHECK_CLOSE(ret.param1(), a, epsilon);
        }
}

ZOB_BEGIN_MODULE(test_tune)

ZOB_CASE(tune_grid)
{
        using scalar_t = double;

        const size_t n_tests = 16;
        const scalar_t minlog = -6.0;
        const scalar_t maxlog = +6.0;
        const scalar_t epslog = zob::epsilon0<scalar_t>();

        for (size_t t = 0; t < n_tests; ++ t)
        {
                zob::random_t<scalar_t> agen(+0.1, +1.0);
                zob::random_t<scalar_t> bgen(+0.2, +2.0);

                check(agen(), bgen(), minlog, maxlog, epslog);
        }
}

ZOB_CASE(tune_finite)
{
        const auto op1 = [] (const auto param1)
        {
                return param1 * param1;
        };
        const auto op2 = [&] (const auto param1, const auto param2)
        {
                return op1(param1) + op1(param2);
        };
        const auto op3 = [&] (const auto param1, const auto param2, const auto param3)
        {
                return op2(param1, param2) + op1(param3);
        };
        const auto op4 = [&] (const auto param1, const auto param2, const auto param3, const auto param4)
        {
                return op3(param1, param2, param3) + op1(param4);
        };

        const auto params1 = zob::make_finite_space(0, 1);
        const auto params2 = zob::make_finite_space(3, 2, 1);
        const auto params3 = zob::make_finite_space(2, 3, 4, 5);
        const auto params4 = zob::make_finite_space(7, 6, 5, 4, 3);

        const auto ret1 = zob::tune(op1, params1);
        const auto ret2 = zob::tune(op2, params1, params2);
        const auto ret3 = zob::tune(op3, params1, params2, params3);
        const auto ret4 = zob::tune(op4, params1, params2, params3, params4);

        ZOB_CHECK_EQUAL(ret1.optimum(), op1(0));
        ZOB_CHECK_EQUAL(ret1.param0(), 0);

        ZOB_CHECK_EQUAL(ret2.optimum(), op2(0, 1));
        ZOB_CHECK_EQUAL(ret2.param0(), 0);
        ZOB_CHECK_EQUAL(ret2.param1(), 1);

        ZOB_CHECK_EQUAL(ret3.optimum(), op3(0, 1, 2));
        ZOB_CHECK_EQUAL(ret3.param0(), 0);
        ZOB_CHECK_EQUAL(ret3.param1(), 1);
        ZOB_CHECK_EQUAL(ret3.param2(), 2);

        ZOB_CHECK_EQUAL(ret4.optimum(), op4(0, 1, 2, 3));
        ZOB_CHECK_EQUAL(ret4.param0(), 0);
        ZOB_CHECK_EQUAL(ret4.param1(), 1);
        ZOB_CHECK_EQUAL(ret4.param2(), 2);
        ZOB_CHECK_EQUAL(ret4.param3(), 3);
}

ZOB_END_MODULE()

