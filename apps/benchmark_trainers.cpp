#include "text/table.h"
#include "text/cmdline.h"
#include "cortex/batch.h"
#include "cortex/cortex.h"
#include "cortex/logger.h"
#include "thread/thread.h"
#include "cortex/measure.hpp"
#include "cortex/minibatch.h"
#include "cortex/stochastic.h"
#include "cortex/accumulator.h"
#include "text/concatenate.hpp"
#include "cortex/tasks/task_charset.h"
#include "cortex/layers/make_layers.h"

using namespace nano;

template
<
        typename tvalue
>
static string_t stats_to_string(const nano::stats_t<tvalue>& stats)
{
        return  nano::to_string(static_cast<tvalue>(stats.avg()))
                + "+/-" + nano::to_string(static_cast<tvalue>(stats.stdev()))
                + " [" + nano::to_string(stats.min())
                + ", " + nano::to_string(stats.max())
                + "]";
}

template
<
        typename ttrainer
>
static void test_optimizer(model_t& model, const string_t& name, const string_t& basepath,
        nano::table_t& table, const vectors_t& x0s, const ttrainer& trainer)
{
        nano::stats_t<scalar_t> errors;
        nano::stats_t<scalar_t> speeds;
        nano::stats_t<scalar_t> timings;

        log_info() << "<<< running " << name << " ...";

        for (size_t i = 0; i < x0s.size(); ++ i)
        {
                const nano::timer_t timer;

                model.load_params(x0s[i]);

                const auto result = trainer();
                const auto opt_state = result.optimum_state();
                const auto opt_speed = nano::convergence_speed(result.optimum_states());

                errors(opt_state.m_test.m_error_avg);
                speeds(opt_speed);
                timings(static_cast<scalar_t>(timer.seconds().count()));

                log_info() << "<<< " << name << ": " << result << ", speed = " << opt_speed << "/s.";

                const auto path = basepath + "-trial" + nano::to_string(i) + ".state";

                const auto opt_states = result.optimum_states();
                nano::save(path, opt_states);
        }

        table.append(name)
                << stats_to_string(errors)
                << stats_to_string(speeds)
                << stats_to_string(timings);
}

static void evaluate(model_t& model,
        const task_t& task, const size_t fold,
        const loss_t& loss, const criterion_t& criterion, const vectors_t& x0s, const size_t iterations,
        const bool use_batch, const bool use_minibatch, const bool use_stochastic,
        const string_t& basename, const string_t& basepath, nano::table_t& table)
{
        const scalar_t epsilon = 1e-4;
        const size_t n_threads = nano::n_threads();
        const bool verbose = true;

        // batch optimizers
        const auto batch_optimizers =
        {
                nano::batch_optimizer::GD,
                nano::batch_optimizer::CGD,
                nano::batch_optimizer::LBFGS
        };

        // minibatch optimizers
        const auto minibatch_optimizers =
        {
                nano::batch_optimizer::GD,
                nano::batch_optimizer::CGD,
                nano::batch_optimizer::LBFGS
        };

        // stochastic optimizers
        const auto stoch_optimizers =
        {
                nano::stoch_optimizer::SG,
                nano::stoch_optimizer::SGM,
                nano::stoch_optimizer::AG,
                nano::stoch_optimizer::AGFR,
                nano::stoch_optimizer::AGGR,
                nano::stoch_optimizer::ADAGRAD,
                nano::stoch_optimizer::ADADELTA,
                nano::stoch_optimizer::ADAM
        };

        // run optimizers and collect results
        if (use_batch)
        for (nano::batch_optimizer optimizer : batch_optimizers)
        {
                const auto optname = "batch-" + nano::to_string(optimizer);
                test_optimizer(model, basename + optname, basepath + optname, table, x0s, [&] ()
                {
                        return  nano::batch_train(model, task, fold,
                                n_threads, loss, criterion, optimizer, iterations, epsilon, verbose);
                });
        }

        if (use_minibatch)
        for (nano::batch_optimizer optimizer : minibatch_optimizers)
        {
                const auto optname = "minibatch-" + nano::to_string(optimizer);
                test_optimizer(model, basename + optname, basepath + optname, table, x0s, [&] ()
                {
                        return  nano::minibatch_train(model, task, fold,
                                n_threads, loss, criterion, optimizer, iterations, epsilon, verbose);
                });
        }

        if (use_stochastic)
        for (nano::stoch_optimizer optimizer : stoch_optimizers)
        {
                const auto optname = "stochastic-" + nano::to_string(optimizer);
                test_optimizer(model, basename + optname, basepath + optname, table, x0s, [&] ()
                {
                        return  nano::stochastic_train(model, task, fold,
                                n_threads, loss, criterion, optimizer, iterations, verbose);
                });
        }
}

int main(int argc, const char* argv[])
{
        nano::init();

        using namespace nano;

        // parse the command line
        nano::cmdline_t cmdline("benchmark trainers");
        cmdline.add("", "mlp0",         "use MLP with 0 hidden layers");
        cmdline.add("", "mlp1",         "use MLP with 1 hidden layers");
        cmdline.add("", "mlp2",         "use MLP with 2 hidden layers");
        cmdline.add("", "mlp3",         "use MLP with 3 hidden layers");
        cmdline.add("", "nmlp0",        "use normalized MLP with 0 hidden layers");
        cmdline.add("", "nmlp1",        "use normalized MLP with 1 hidden layers");
        cmdline.add("", "nmlp2",        "use normalized MLP with 2 hidden layers");
        cmdline.add("", "nmlp3",        "use normalized MLP with 3 hidden layers");
        cmdline.add("", "convnet1",     "use convolution network (conv-pool-conv)");
        cmdline.add("", "convnet2",     "use convolution network (conv-conv)");
        cmdline.add("", "convnet3",     "use convolution network (conv-conv-conv)");
        cmdline.add("", "batch",        "evaluate batch optimizers");
        cmdline.add("", "minibatch",    "evaluate mini-batch optimizers");
        cmdline.add("", "stochastic",   "evaluate stochastic optimizers");
        cmdline.add("", "l2n-reg",      "also evaluate the l2-norm-based regularizer");
        cmdline.add("", "var-reg",      "also evaluate the variance-based regularizer");
        cmdline.add("", "trials",       "number of models to train & evaluate", "10");
        cmdline.add("", "iterations",   "number of iterations/epochs", "64");

        cmdline.process(argc, argv);

        // check arguments and options
        const bool use_mlp0 = cmdline.has("mlp0");
        const bool use_mlp1 = cmdline.has("mlp1");
        const bool use_mlp2 = cmdline.has("mlp2");
        const bool use_mlp3 = cmdline.has("mlp3");
        const bool use_nmlp0 = cmdline.has("nmlp0");
        const bool use_nmlp1 = cmdline.has("nmlp1");
        const bool use_nmlp2 = cmdline.has("nmlp2");
        const bool use_nmlp3 = cmdline.has("nmlp3");
        const bool use_convnet1 = cmdline.has("convnet1");
        const bool use_convnet2 = cmdline.has("convnet2");
        const bool use_convnet3 = cmdline.has("convnet3");
        const bool use_batch = cmdline.has("batch");
        const bool use_minibatch = cmdline.has("minibatch");
        const bool use_stochastic = cmdline.has("stochastic");
        const bool use_reg_l2n = cmdline.has("l2n-reg");
        const bool use_reg_var = cmdline.has("var-reg");
        const auto trials = cmdline.get<size_t>("trials");
        const auto iterations = cmdline.get<size_t>("iterations");

        if (    !use_mlp0 &&
                !use_mlp1 &&
                !use_mlp2 &&
                !use_mlp3 &&
                !use_nmlp0 &&
                !use_nmlp1 &&
                !use_nmlp2 &&
                !use_nmlp3 &&
                !use_convnet1 &&
                !use_convnet2 &&
                !use_convnet3)
        {
                cmdline.usage();
        }

        if (    !use_batch &&
                !use_minibatch &&
                !use_stochastic)
        {
                cmdline.usage();
        }

        // create task
        const size_t rows = 16;
        const size_t cols = 16;
        const size_t count = nano::n_threads() * 32 * 100;
        const color_mode color = color_mode::rgb;

        charset_task_t task(charset::digit, color, rows, cols, count);
        task.load();

        const size_t fold = 0;
        const auto outputs = task.osize();

        // construct models
        const auto mlp0 = string_t();
        const auto mlp1 = mlp0 + make_affine_layer(16);
        const auto mlp2 = mlp1 + make_affine_layer(16);
        const auto mlp3 = mlp2 + make_affine_layer(16);

        const auto nmlp0 = string_t();
        const auto nmlp1 = nmlp0 + make_norm_affine_layer(16);
        const auto nmlp2 = nmlp1 + make_norm_affine_layer(16);
        const auto nmlp3 = nmlp2 + make_norm_affine_layer(16);

        const auto convnet1 =
                make_conv_pool_layer(16, 7, 7);

        const auto convnet2 =
                make_conv_pool_layer(16, 7, 7) +
                make_conv_layer(16, 5, 5);

        const auto convnet3 =
                make_conv_layer(16, 7, 7) +
                make_conv_layer(16, 5, 5) +
                make_conv_layer(16, 3, 3);

        const string_t outlayer = make_output_layer(outputs);

        std::vector<std::pair<string_t, string_t>> networks;
        #define DEFINE(config) networks.emplace_back(config + outlayer, NANO_STRINGIFY(config))

        if (use_mlp0) { DEFINE(mlp0); }
        if (use_mlp1) { DEFINE(mlp1); }
        if (use_mlp2) { DEFINE(mlp2); }
        if (use_mlp3) { DEFINE(mlp3); }
        if (use_nmlp0) { DEFINE(nmlp0); }
        if (use_nmlp1) { DEFINE(nmlp1); }
        if (use_nmlp2) { DEFINE(nmlp2); }
        if (use_nmlp3) { DEFINE(nmlp3); }
        if (use_convnet1) { DEFINE(convnet1); }
        if (use_convnet2) { DEFINE(convnet2); }
        if (use_convnet3) { DEFINE(convnet3); }

        #undef DEFINE

        const strings_t losses = { "classnll" }; //nano::get_losses().ids();

        strings_t criteria;
        criteria.push_back("avg"); //nano::get_criteria().ids();
        if (use_reg_l2n) { criteria.push_back("l2n-reg"); }
        if (use_reg_var) { criteria.push_back("var-reg"); }

        // vary the model
        for (const auto& net : networks)
        {
                const auto& network = net.first;
                const auto& netname = net.second;

                log_info() << "<<< running network [" << network << "] ...";

                const auto model = nano::get_models().get("forward-network", network);
                model->resize(task, true);

                // generate fixed random starting points
                vectors_t x0s(trials);
                for (vector_t& x0 : x0s)
                {
                        model->random_params();
                        model->save_params(x0);
                }

                // vary the loss
                for (const string_t& iloss : losses)
                {
                        log_info() << "<<< running loss [" << iloss << "] ...";

                        const auto loss = nano::get_losses().get(iloss);

                        nano::table_t table("optimizer");
                        table.header() << "test error"
                                       << "convergence speed"
                                       << "time [sec]";

                        // vary the criteria
                        for (const string_t& icriterion : criteria)
                        {
                                const auto criterion = nano::get_criteria().get(icriterion);
                                const auto basename = "[" + icriterion + "] ";
                                const auto basepath = netname + "-" + iloss + "-" + icriterion + "-";

                                evaluate(*model, task, fold, *loss, *criterion, x0s, iterations,
                                         use_batch, use_minibatch, use_stochastic,
                                         basename, basepath, table);
                        }

                        // show results
                        table.print(std::cout);
                }

                log_info();
        }

        // OK
        log_info() << done;
        return EXIT_SUCCESS;
}
