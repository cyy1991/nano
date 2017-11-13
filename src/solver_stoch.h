#pragma once

#include "factory.h"
#include "function.h"
#include "stoch/params.h"

namespace nano
{
        ///
        /// \brief stores registered prototypes
        ///
        class stoch_solver_t;
        using stoch_solver_factory_t = factory_t<stoch_solver_t>;
        using rstoch_solver_t = stoch_solver_factory_t::trobject;

        NANO_PUBLIC stoch_solver_factory_t& get_stoch_solvers();

        ///
        /// \brief generic stochastic solver that used an apriori learning-rate schedule.
        /// NB: all its hyper-parameters are tuned automatically.
        ///
        class NANO_PUBLIC stoch_solver_t
        {
        public:

                virtual ~stoch_solver_t() = default;

                ///
                /// \brief serialize the current parameters to json
                ///
                virtual void config(json_reader_t&) = 0;
                virtual void config(json_writer_t&) const = 0;

                ///
                /// \brief minimize starting from the initial point x0.
                ///
                virtual function_state_t minimize(const stoch_params_t&, const function_t&, const vector_t& x0) const = 0;
        };
}
