#pragma once

#include "enhancer.h"

namespace nano
{
        ///
        /// \brief use the samples as they are.
        ///
        struct enhancer_default_t final : public enhancer_t
        {
                explicit enhancer_default_t(const string_t& params = string_t());

                virtual sample_t get(const task_t&, const fold_t&, const size_t index) const final;
        };
}