#include "function.h"
#include "text/cmdline.h"
#include "math/epsilon.h"
#include "solver_batch.h"
#include "benchmark_solvers.h"

using namespace nano;

template <typename tostats>
static void check_function(const function_t& function, const strings_t& solvers,
        const size_t trials, const size_t iterations, const scalar_t epsilon, const scalar_t c1, tostats& gstats)
{
        // generate fixed random trials
        std::vector<vector_t> x0s(trials);
        for (auto& x0 : x0s)
        {
                x0 = vector_t::Random(function.size());
        }

        // per-problem statistics
        tostats stats;

        // evaluate all possible combinations (solver & line-search)
        for (const auto& id : solvers)
                for (const ls_initializer ls_init : enum_values<ls_initializer>())
                        for (const ls_strategy ls_strat : enum_values<ls_strategy>())
        {
                const auto solver = get_batch_solvers().get(id);
                solver->from_json(to_json("ls_init", ls_init, "ls_strat", ls_strat, "c1", c1));
                const auto params = batch_params_t(iterations, epsilon);
                const auto name = id + "[" + to_string(ls_init) + "][" + to_string(ls_strat) + "]";

                for (const auto& x0 : x0s)
                {
                        benchmark::benchmark_function(
                                [&] () { return solver->minimize(params, function, x0); },
                                function, x0, name, stats, gstats);
                }
        }

        // show per-problem statistics
        benchmark::show_table(function.name(), stats);
}

int main(int argc, const char* argv[])
{
        using namespace nano;

        // parse the command line
        cmdline_t cmdline("benchmark batch solvers");
        cmdline.add("", "solvers",      "use this regex to select the solvers to benchmark", ".+");
        cmdline.add("", "functions",    "use this regex to select the functions to benchmark", ".+");
        cmdline.add("", "min-dims",     "minimum number of dimensions for each test function (if feasible)", "100");
        cmdline.add("", "max-dims",     "maximum number of dimensions for each test function (if feasible)", "1000");
        cmdline.add("", "trials",       "number of random trials for each test function", "100");
        cmdline.add("", "iterations",   "maximum number of iterations", "1000");
        cmdline.add("", "epsilon",      "convergence criteria", 1e-6);
        cmdline.add("", "convex",       "use only convex test functions");
        cmdline.add("", "c1",           "sufficient decrease coefficient (Wolfe conditions)", 1e-4);

        cmdline.process(argc, argv);

        // check arguments and options
        const auto min_dims = cmdline.get<tensor_size_t>("min-dims");
        const auto max_dims = cmdline.get<tensor_size_t>("max-dims");
        const auto trials = cmdline.get<size_t>("trials");
        const auto iterations = cmdline.get<size_t>("iterations");
        const auto epsilon = cmdline.get<scalar_t>("epsilon");
        const auto is_convex = cmdline.has("convex");
        const auto c1 = cmdline.get<scalar_t>("c1");

        const auto solvers = get_batch_solvers().ids(std::regex(cmdline.get<string_t>("solvers")));
        const auto functions = std::regex(cmdline.get<string_t>("functions"));

        std::map<std::string, benchmark::solver_stat_t> gstats;

        for (const auto& function : (is_convex ? get_convex_functions : get_functions)(min_dims, max_dims, functions))
        {
                check_function(*function, solvers, trials, iterations, epsilon, c1, gstats);
        }

        // show global statistics
        benchmark::show_table(std::string(), gstats);

        // show per-solver statistics
        for (const auto& solver : solvers)
        {
                const auto name = solver + "[";

                std::map<std::string, benchmark::solver_stat_t> stats;
                for (const auto& gstat : gstats)
                {
                        if (starts_with(gstat.first, name))
                        {
                                stats[gstat.first] = gstat.second;
                        }
                }

                benchmark::show_table(std::string(), stats);
        }

        // OK
        return EXIT_SUCCESS;
}
