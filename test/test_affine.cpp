#include "loss.h"
#include "utest.h"
#include "trainer.h"
#include "criterion.h"
#include "math/epsilon.h"
#include "tasks/task_affine.h"
#include "layers/make_layers.h"

using namespace nano;

const auto idims = tensor_size_t(3);
const auto irows = tensor_size_t(5);
const auto icols = tensor_size_t(7);
const auto osize = tensor_size_t(4);
const auto count = tensor_size_t(1000);
const auto noise = epsilon2<scalar_t>();

NANO_BEGIN_MODULE(test_affine)

NANO_CASE(construction)
{
        affine_task_t task(to_params(
                "idims", idims, "irows", irows, "icols", icols, "osize", osize, "count", count, "noise", noise,
                "mode", affine_mode::regression));

        task.load();

        NANO_CHECK_EQUAL(task.idims(), idims);
        NANO_CHECK_EQUAL(task.irows(), irows);
        NANO_CHECK_EQUAL(task.icols(), icols);
        NANO_CHECK_EQUAL(task.osize(), osize);
        NANO_CHECK_EQUAL(task.n_samples(), count);
        NANO_REQUIRE_EQUAL(task.n_folds(), size_t(1));

        const auto& weights = task.weights();
        const auto& bias = task.bias();

        for (const auto proto : {protocol::train, protocol::valid, protocol::test})
        {
                const auto fold = fold_t{0, proto};
                const auto size = task.n_samples(fold);
                for (size_t i = 0; i < size; ++ i)
                {
                        const auto input = task.input(fold, i);
                        const auto target = task.target(fold, i);
                        NANO_CHECK_EIGEN_CLOSE(weights * input.vector() + bias, target, 2 * noise);
                }
        }
}

NANO_CASE(regression)
{
        affine_task_t task(to_params(
                "idims", idims, "irows", irows, "icols", icols, "osize", osize, "count", count, "noise", noise,
                "mode", affine_mode::regression));

        task.load();

        // create model
        const auto model_config = make_output_layer(task.osize());
        const auto model = get_models().get("forward-network", model_config);
        NANO_CHECK_EQUAL(model->resize(task, true), true);
        NANO_REQUIRE(*model == task);

        // create loss
        std::vector<rloss_t> losses;
        const auto add_loss = [&] (const auto& id, const auto& params)
        {
                losses.push_back(std::move(get_losses().get(id, params)));
        };
        add_loss("square", "");
        add_loss("cauchy", "");

        // create criteria
        const auto criterion = get_criteria().get("avg");

        // create trainers
        const auto batch = 32;
        const auto epochs = 1000;
        const auto policy = trainer_policy::stop_early;
        std::vector<rtrainer_t> trainers;
        const auto add_trainer = [&] (const auto& id, const auto& params)
        {
                trainers.push_back(std::move(get_trainers().get(id, params)));
        };
        add_trainer("batch", to_params("opt", "gd", "epochs", epochs, "policy", policy));
        add_trainer("batch", to_params("opt", "cgd", "epochs", epochs, "policy", policy));
        add_trainer("batch", to_params("opt", "lbfgs", "epochs", epochs, "policy", policy));
        add_trainer("stoch", to_params("opt", "sg", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        add_trainer("stoch", to_params("opt", "sgm", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        add_trainer("stoch", to_params("opt", "ngd", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "adam", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "adagrad", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "adadelta", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "ag", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "agfr", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "aggr", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));

        // check training
        for (const auto& loss : losses)
        {
                for (const auto& trainer : trainers)
                {
                        model->random_params();

                        const auto fold = size_t(0);
                        const auto threads = size_t(1);
                        const auto result = trainer->train(task, fold, threads, *loss, *criterion, *model);

                        // the average training loss value & error should be "small"
                        const auto opt_state = result.optimum_state();
                        NANO_CHECK_LESS(opt_state.m_train.m_value_avg, epsilon2<scalar_t>());
                        NANO_CHECK_LESS(opt_state.m_train.m_error_avg, epsilon3<scalar_t>());
                }
        }
}

NANO_CASE(classification)
{
        affine_task_t task(to_params(
                "idims", idims, "irows", irows, "icols", icols, "osize", osize, "count", count, "noise", noise,
                "mode", affine_mode::sign_class));

        task.load();

        // create model
        const auto model_config = make_output_layer(task.osize());
        const auto model = get_models().get("forward-network", model_config);
        NANO_CHECK_EQUAL(model->resize(task, true), true);
        NANO_REQUIRE(*model == task);

        // create loss
        std::vector<rloss_t> losses;
        const auto add_loss = [&] (const auto& id, const auto& params)
        {
                losses.push_back(std::move(get_losses().get(id, params)));
        };
        add_loss("logistic", "");
        add_loss("exponential", "");

        // create criteria
        const auto criterion = get_criteria().get("avg");

        // create trainers
        const auto batch = 32;
        const auto epochs = 1000;
        const auto policy = trainer_policy::stop_early;
        std::vector<rtrainer_t> trainers;
        const auto add_trainer = [&] (const auto& id, const auto& params)
        {
                trainers.push_back(std::move(get_trainers().get(id, params)));
        };
        add_trainer("batch", to_params("opt", "gd", "epochs", epochs, "policy", policy));
        add_trainer("batch", to_params("opt", "cgd", "epochs", epochs, "policy", policy));
        add_trainer("batch", to_params("opt", "lbfgs", "epochs", epochs, "policy", policy));
        add_trainer("stoch", to_params("opt", "sg", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        add_trainer("stoch", to_params("opt", "sgm", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        add_trainer("stoch", to_params("opt", "ngd", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "adam", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "adagrad", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "adadelta", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "ag", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "agfr", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));
        //add_trainer("stoch", to_params("opt", "aggr", "epochs", epochs, "min_batch", batch, "max_batch", batch, "policy", policy));

        // check training
        for (const auto& loss : losses)
        {
                for (const auto& trainer : trainers)
                {
                        model->random_params();

                        const auto fold = size_t(0);
                        const auto threads = size_t(1);
                        const auto result = trainer->train(task, fold, threads, *loss, *criterion, *model);

                        // the average training loss value & error should be "small"
                        const auto opt_state = result.optimum_state();
                        NANO_CHECK_LESS(opt_state.m_train.m_value_avg, epsilon2<scalar_t>());
                        NANO_CHECK_LESS(opt_state.m_train.m_error_avg, epsilon3<scalar_t>());
                }
        }
}

NANO_END_MODULE()