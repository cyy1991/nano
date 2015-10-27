#pragma once

#include "buffer.h"

namespace file
{
        class mstream_t;

        ///
        /// \brief uncompress a stream of bytes (using bzip2)
        ///
        NANOCV_PUBLIC bool uncompress_bzip2(std::istream& istream, std::streamsize num_bytes, buffer_t&);
        NANOCV_PUBLIC bool uncompress_bzip2(std::istream& istream, buffer_t&);
        NANOCV_PUBLIC bool uncompress_bzip2(mstream_t& istream, buffer_t&);
}