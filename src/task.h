#pragma once

#include "arch.h"
#include "cortex.h"
#include "factory.h"

namespace nano
{
        ///
        /// \brief manage tasks (register new ones, query and clone them)
        ///
        class task_t;
        using task_factory_t = factory_t<task_t>;
        using rtask_t = task_factory_t::trobject;

        NANO_PUBLIC task_factory_t& get_tasks();

        ///
        /// \brief print a short description of a task.
        ///
        NANO_PUBLIC void describe(const task_t& task, const string_t& name);

        ///
        /// \brief check task for consistency per fold index:
        ///     ideally there should be no duplications per fold index.
        /// \return maximum number of duplicates
        ///
        NANO_PUBLIC size_t check_duplicates(const task_t& task);

        ///
        /// \brief check task for consistency per fold index:
        ///     ideally there should be no intersection between training, validation and tests datasets.
        /// \return maximum number of duplicates between folds
        ///
        NANO_PUBLIC size_t check_intersection(const task_t& task);

        ///
        /// \brief machine learning task consisting of a collection of fixed-size 3D input tensors
        ///     split into training, validation and testing datasets.
        /// NB: the samples may be organized in folds depending on the established protocol.
        ///
        class NANO_PUBLIC task_t
        {
        public:

                virtual ~task_t() = default;

                ///
                /// \brief serialize the current parameters to json
                ///
                virtual bool config(json_reader_t&) = 0;
                virtual void config(json_writer_t&) const = 0;

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
                /// \brief retrieve the given [begin, end) range of samples as a minibatch
                ///
                virtual minibatch_t get(const fold_t&, const size_t begin, const size_t end) const = 0;

                ///
                /// \brief retrieve the hash for a given input/target
                ///
                virtual size_t ihash(const fold_t&, const size_t index) const = 0;
                virtual size_t ohash(const fold_t&, const size_t index) const = 0;

                ///
                /// \brief retrieve the label (if available) for a given sample
                ///
                virtual string_t label(const fold_t&, const size_t index) const = 0;
        };
}
