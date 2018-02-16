#include "text/table.h"
#include "math/stats.h"
#include "math/numeric.h"
#include "thread/loopi.h"
#include "solver_state.h"
#include "text/algorithm.h"
#include <iomanip>
#include <iostream>

namespace benchmark
{
        using namespace nano;

        struct solver_stat_t
        {
                stats_t<scalar_t> m_crits;      ///< convergence criteria
                stats_t<scalar_t> m_fails;      ///< #convergence failures
                stats_t<scalar_t> m_fcalls;     ///< #function value calls
                stats_t<scalar_t> m_gcalls;     ///< #gradient calls
                stats_t<scalar_t> m_speeds;     ///< #convergence speeds
        };

        static void show_table(const string_t& table_name, const std::map<string_t, solver_stat_t>& ostats)
        {
                assert(!ostats.empty());

                // show global statistics
                table_t table;
                table.header()
                        << nano::align(table_name.empty() ? "solver" : table_name, 32)
                        << "cost"
                        << "|g|/(1+|f|)"
                        << "#fails"
                        << "#fcalls"
                        << "#gcalls"
                        << "speed";
                table.delim();

                for (const auto& it : ostats)
                {
                        const auto& name = it.first;
                        const auto& stat = it.second;

                        if (stat.m_fcalls)
                        {
                                table.append()
                                << name
                                << static_cast<size_t>(stat.m_fcalls.avg() + 2 * stat.m_gcalls.avg())
                                << stat.m_crits.avg()
                                << static_cast<size_t>(stat.m_fails.sum1())
                                << static_cast<size_t>(stat.m_fcalls.avg())
                                << static_cast<size_t>(stat.m_gcalls.avg())
                                << stat.m_speeds.avg();
                        }
                }

                table.sort(nano::make_less_from_string<scalar_t>(), {2, 0});
                std::cout << table;
        }

        template <typename tsolver, typename tostats>
        static void benchmark_function(const tsolver& solver,
                const function_t& function, const vector_t& x0, const string_t& name,
                tostats& stats, tostats& gstats)
        {
                solver_state_t state0(function.size());
                state0.update(function, x0);
                const auto g0 = state0.convergence_criteria();

                // optimize
                const auto old_fcalls = function.fcalls();
                const auto old_gcalls = function.gcalls();

                const auto state = solver();

                const auto fcalls = function.fcalls() - old_fcalls;
                const auto gcalls = function.gcalls() - old_gcalls;

                const auto g = state.convergence_criteria();
                const auto speed = std::pow(
                        static_cast<double>(epsilon0<scalar_t>() + g) /
                        static_cast<double>(epsilon0<scalar_t>() + g0),
                        double(1) / double(gcalls));

                // ignore out-of-domain solutions
                if (state && function.is_valid(state.x))
                {
                        // update per-function statistics
                        solver_stat_t& stat = stats[name];
                        stat.m_crits(g);
                        stat.m_fails(state.m_status != opt_status::converged ? 1 : 0);
                        stat.m_fcalls(static_cast<scalar_t>(fcalls));
                        stat.m_gcalls(static_cast<scalar_t>(gcalls));
                        stat.m_speeds(static_cast<scalar_t>(speed));

                        // update global statistics
                        solver_stat_t& gstat = gstats[name];
                        gstat.m_crits(g);
                        gstat.m_fails(state.m_status != opt_status::converged ? 1 : 0);
                        gstat.m_fcalls(static_cast<scalar_t>(fcalls));
                        gstat.m_gcalls(static_cast<scalar_t>(gcalls));
                        gstat.m_speeds(static_cast<scalar_t>(speed));
                }
        }
}
