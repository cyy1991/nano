#include "function.h"
#include "core/epsilon.h"
#include "functions/trid.h"
#include "functions/qing.h"
#include "functions/cauchy.h"
#include "functions/sphere.h"
#include "functions/powell.h"
#include "functions/sargan.h"
#include "functions/zakharov.h"
#include "functions/quadratic.h"
#include "functions/rosenbrock.h"
#include "functions/exponential.h"
#include "functions/dixon_price.h"
#include "functions/axis_ellipsoid.h"
#include "functions/chung_reynolds.h"
#include "functions/styblinski_tang.h"
#include "functions/rotated_ellipsoid.h"
#include "functions/schumer_steiglitz.h"

using namespace nano;

function_t::function_t(const char* name,
        const tensor_size_t size, const tensor_size_t min_size, const tensor_size_t max_size,
        const convexity convex,
        const scalar_t domain) :
        m_name(name),
        m_size(size), m_min_size(min_size), m_max_size(max_size),
        m_convex(convex),
        m_domain(domain)
{
}

scalar_t function_t::eval(const vector_t& x, vector_t* gx) const
{
        assert(x.size() == size());

        if (gx)
        {
                gx->resize(size());
        }

        const auto f = vgrad(x, gx);
        assert(!gx || gx->size() == size());

        m_fcalls += 1;
        m_gcalls += gx ? 1 : 0;

        return f;
}

scalar_t function_t::grad_accuracy(const vector_t& x) const
{
        assert(x.size() == size());

        const auto n = size();

        vector_t gx(n);
        vector_t gx_approx(n);
        vector_t xp = x, xn = x;

        // analytical gradient
        const auto fx = vgrad(x, &gx);
        assert(gx.size() == size());

        // finite-difference approximated gradient
        //      see "Numerical optimization", Nocedal & Wright, 2nd edition, p.197
        const auto finite_difference = [&] (const scalar_t dx)
        {
                for (auto i = 0; i < n; i ++)
                {
                        xp = x; xp(i) += dx * (1 + std::fabs(x(i)));
                        xn = x; xn(i) -= dx * (1 + std::fabs(x(i)));

                        const auto dfi = vgrad(xp, nullptr) - vgrad(xn, nullptr);
                        const auto dxi = xp(i) - xn(i);
                        gx_approx(i) = dfi / dxi;

                        assert(std::isfinite(gx(i)));
                        assert(std::isfinite(gx_approx(i)));
                }

                return (gx - gx_approx).lpNorm<Eigen::Infinity>() / (1 + std::fabs(fx));
        };

        return finite_difference(epsilon2<scalar_t>());
}

bool function_t::is_convex(const vector_t& x1, const vector_t& x2, const int steps) const
{
        assert(steps > 2);
        assert(x1.size() == size());
        assert(x2.size() == size());

        const auto f1 = vgrad(x1, nullptr);
        assert(std::isfinite(f1));

        const auto f2 = vgrad(x2, nullptr);
        assert(std::isfinite(f2));

        for (int step = 1; step < steps; step ++)
        {
                const auto t1 = scalar_t(step) / scalar_t(steps);
                const auto t2 = scalar_t(1) - t1;
                const auto ftc = t1 * f1 + t2 * f2;

                const auto ft = vgrad(t1 * x1 + t2 * x2, nullptr);
                if (std::isfinite(ft) && ft > ftc + epsilon0<scalar_t>() * (1 + std::fabs(ftc)))
                {
                        return false;
                }
        }

        return true;
}

string_t function_t::name() const
{
        return string_t(m_name) + "[" + std::to_string(size()) + "D]";
}

bool function_t::is_valid(const vector_t& x) const
{
        return x.lpNorm<Eigen::Infinity>() < m_domain;
}

static void append(rfunction_t&& func, const tensor_size_t dims, const std::regex& regex, rfunctions_t& funcs)
{
        if (func->min_size() <= dims && dims <= func->max_size() && std::regex_match(func->name(), regex))
        {
                funcs.push_back(std::move(func));
        }
}

rfunctions_t nano::get_functions(const tensor_size_t min_size, const tensor_size_t max_size, const std::regex& regex)
{
        assert(min_size >= 1);
        assert(min_size <= max_size);

        rfunctions_t funcs;
        for (tensor_size_t dims = min_size; dims <= max_size; )
        {
                append(std::make_unique<function_trid_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_qing_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_cauchy_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_sargan_t>(dims), dims, regex, funcs);
                if (dims % 4 == 0)
                {
                        append(std::make_unique<function_powell_t>(dims), dims, regex, funcs);
                }
                append(std::make_unique<function_sphere_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_zakharov_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_quadratic_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_rosenbrock_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_exponential_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_dixon_price_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_chung_reynolds_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_axis_ellipsoid_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_styblinski_tang_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_schumer_steiglitz_t>(dims), dims, regex, funcs);
                append(std::make_unique<function_rotated_ellipsoid_t>(dims), dims, regex, funcs);

                if (dims < 8)
                {
                        ++ dims;
                }
                else
                {
                        dims *= 2;
                }
        }

        return funcs;
}

rfunctions_t nano::get_convex_functions(const tensor_size_t min_size, const tensor_size_t max_size, const std::regex& regex)
{
        auto funcs = get_functions(min_size, max_size, regex);

        funcs.erase(
                std::remove_if(funcs.begin(), funcs.end(), [] (const auto& func) { return !func->is_convex(); }),
                funcs.end());

        return funcs;
}
