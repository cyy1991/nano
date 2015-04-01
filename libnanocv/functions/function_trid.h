#pragma once

#include "function.h"

namespace ncv
{
        ///
        /// \brief create Trid function tests
        ///
        std::vector<function_t> make_trid_funcs(ncv::size_t max_dims = 32);
}
