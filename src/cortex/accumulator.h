#pragma once

#include "target.h"
#include "criterion.h"

namespace nano
{
        ///
        /// \brief cumulate sample evaluations (loss value, error and gradient)
        ///
        class NANO_PUBLIC accumulator_t
        {
        public:

                ///
                /// \brief constructor
                ///
                accumulator_t(const model_t&, const criterion_t&, criterion_t::type,
                              scalar_t lambda = 0.0);

                ///
                /// \brief disable copying
                ///
                accumulator_t(const accumulator_t&) = delete;
                accumulator_t& operator=(const accumulator_t&) = delete;

                ///
                /// \brief destructor
                ///
                ~accumulator_t();

                ///
                /// \brief reset accumulator (keeps parameters)
                ///
                void reset() const;

                ///
                /// \brief change the model's parameters (and resets statistics)
                ///
                void set_params(const vector_t& params) const;

                ///
                /// \brief change the regularization weight (keeps parameters)
                ///
                void set_lambda(scalar_t lambda) const;

                ///
                /// \brief change the number of active threads
                ///
                void set_threads(size_t nthreads) const;

                ///
                /// \brief cumulate statistics with a set of samples
                ///
                void update(const task_t&, const fold_t&, const loss_t&);
                void update(const task_t&, const fold_t&, const size_t begin, const size_t end, const loss_t&);

                ///
                /// \brief cumulated loss value
                ///
                scalar_t value() const;

                ///
                /// \brief cumulated gradient
                ///
                vector_t vgrad() const;

                ///
                /// \brief averaged error value
                ///
                scalar_t avg_error() const;

                ///
                /// \brief variance error value
                ///
                scalar_t var_error() const;

                ///
                /// \brief total number of processed samples
                ///
                size_t count() const;

                ///
                /// \brief number of dimensions/parameters
                ///
                tensor_size_t psize() const;

                ///
                /// \brief regularization weight (if any)
                ///
                scalar_t lambda() const;

                ///
                /// \brief check if the criterion has a regularization term to tune
                ///
                bool can_regularize() const;

        private:

                // attributes
                struct impl_t;
                std::unique_ptr<impl_t> m_impl;
        };
}
