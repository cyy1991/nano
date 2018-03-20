#include "utest.h"
#include "builder.h"
#include "trainer.h"
#include "accumulator.h"
#include "math/epsilon.h"
#include "solver_stoch.h"
#include "tasks/task_affine.h"

using namespace nano;

NANO_BEGIN_MODULE(trainer_stoch_affine)

NANO_CASE(tune_and_train)
{
        const auto isize = 3;
        const auto osize = 2;
        const auto fold = 0;

        // create synthetic task
        const auto task = get_tasks().get("synth-affine");
        NANO_REQUIRE(task);
        task->from_json(to_json("isize", isize, "osize", osize, "noise", 0, "count", 100));
        NANO_REQUIRE(task->load());
        NANO_REQUIRE_EQUAL(task->idims(), make_dims(isize, 1, 1));
        NANO_REQUIRE_EQUAL(task->odims(), make_dims(osize, 1, 1));

        // create loss
        const auto loss = get_losses().get("square");
        NANO_REQUIRE(loss);

        // create model
        model_t model;
        NANO_REQUIRE(model.add(config_affine_node("fc", osize, 1, 1)));
        NANO_REQUIRE(model.done());
        NANO_REQUIRE(model.resize(make_dims(isize, 1, 1), make_dims(osize, 1, 1)));

        // create stochastic trainer
        const auto trainer = get_trainers().get("stoch");
        NANO_REQUIRE(trainer);

        // check that the trainer works for all compatible solvers
        for (const auto& solver : get_stoch_solvers().ids())
        {
                if (solver == "cocob")  // todo: make COCOB work!
                        continue;

                trainer->from_json(to_json("epochs", 50, "solver", solver, "epsilon", epsilon2<scalar_t>()));

                accumulator_t acc(model, *loss);
                acc.threads(1);

                acc.random();
                const auto result = trainer->train(*task, fold, acc);
                NANO_REQUIRE(result);

                const auto state = result.optimum_state();
                NANO_CHECK_LESS(state.m_train.m_value, epsilon2<scalar_t>());
                NANO_CHECK_LESS(state.m_valid.m_value, epsilon2<scalar_t>());
                NANO_CHECK_LESS(state.m_test.m_value, epsilon2<scalar_t>());
        }
}

NANO_END_MODULE()