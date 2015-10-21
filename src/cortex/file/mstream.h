#pragma once

#include "arch.h"
#include <ios>
#include <string>
#include <cstddef>

namespace cortex
{
        ///
        /// \brief map the std::istream's interface over a fixed-size in-memory buffer
        ///
        class NANOCV_PUBLIC mstream_t
        {
        public:

                ///
                /// \brief constructor
                ///
                template
                <
                        typename tsize
                >
                mstream_t(const char* data, const tsize size)
                        :       m_data(data),
                                m_size(static_cast<std::streamsize>(size)),
                                m_tellg(0),
                                m_gcount(0)
                {
                }

                ///
                /// \brief disable copying
                ///
                mstream_t(const mstream_t&) = delete;
                mstream_t& operator=(const mstream_t&) = delete;

                ///
                /// \brief read given number of bytes
                ///
                bool read(char* bytes, std::streamsize max_num_bytes);

                ///
                /// \brief read POD structure
                ///
                template
                <
                        typename tstruct
                >
                bool read(tstruct& pod)
                {
                        return read(reinterpret_cast<char*>(&pod), sizeof(pod));
                }

                ///
                /// \brief read next line
                ///
                bool getline(std::string& line);

                ///
                /// \brief skip the given number of bytes
                ///
                bool skip(std::streamsize num_bytes);

                ///
                /// \brief number of bytes read at the last operation
                ///
                std::streamsize gcount() const;

                ///
                /// \brief current position in the buffer
                ///
                std::streamsize tellg() const;

                ///
                /// \brief buffer size
                ///
                std::streamsize size() const;

                ///
                /// \brief check if EOF
                ///
                operator bool() const;

                ///
                /// \brief buffer data
                ///
                const char* data() const;

        private:

                const char* const       m_data;
                std::streamsize         m_size;
                std::streamsize         m_tellg;
                std::streamsize         m_gcount;
        };
}
