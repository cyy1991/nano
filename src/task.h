#pragma once

#include "arch.h"
#include "cortex.h"
#include "factory.h"

namespace nano
{
        ///
        /// \brief manage tasks (register new ones, query and clone them)
        ///
        struct task_t;
        using task_factory_t = factory_t<task_t>;
        using rtask_t = task_factory_t::trobject;

        NANO_PUBLIC task_factory_t& get_tasks();

        ///
        /// \brief machine learning task consisting of a collection of fixed-size 3D input tensors
        ///     split into training, validation and testing datasets.
        /// NB: the samples may be organized in folds depending on the established protocol.
        ///
        struct NANO_PUBLIC task_t : public configurable_t
        {
                using configurable_t::configurable_t;

                ///
                /// \brief populate task with samples
                ///
                virtual bool load() = 0;

                ///
                /// \brief input size
                ///
                virtual tensor3d_dims_t idims() const = 0;

                ///
                /// \brief output size
                ///
                virtual tensor3d_dims_t odims() const = 0;

                ///
                /// \brief number of folds
                ///
                virtual size_t fsize() const = 0;

                ///
                /// \brief total number of samples
                ///
                virtual size_t size() const = 0;

                ///
                /// \brief number of samples for the given fold
                ///
                virtual size_t size(const fold_t&) const = 0;

                ///
                /// \brief randomly shuffle the samples associated for the given fold
                ///
                virtual void shuffle(const fold_t&) const = 0;

                ///
                /// \brief retrieve the given sample
                ///
                virtual sample_t get(const fold_t&, const size_t index) const = 0;

                ///
                /// \brief retrieve the hash for a given sample
                ///
                virtual size_t hash(const fold_t&, const size_t index) const = 0;
        };
}
