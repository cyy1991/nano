#include "archive.h"
#include <archive.h>
#include <archive_entry.h>

namespace nano
{
        archive_stream_t::archive_stream_t(archive* ar) :
                m_archive(ar),
                m_index(0)
        {
        }

        bool archive_stream_t::read(char* data, const std::streamsize num_bytes)
        {
                // buffer data
                const void* buff = nullptr;
                size_t size = 0;
                while (m_index + num_bytes > static_cast<std::streamsize>(m_buffer.size()))
                {
                        if (!advance(buff, size))
                        {
                                return false;
                        }
                        else
                        {
                                m_buffer.insert(m_buffer.end(), (const char*)buff, (const char*)buff + size);
                        }
                }

                // retrive
                std::copy(m_buffer.data() + m_index, m_buffer.data() + (m_index + num_bytes), data);
                m_index += num_bytes;

                // keep the buffer small enough
                const auto max_buffer_size = size_t(1024) * size_t(1024);
                if (m_buffer.size() > max_buffer_size)
                {
                        m_buffer.erase(m_buffer.begin(), m_buffer.begin() + m_index);
                        m_index = 0;
                }
                return true;
        }

        static bool isendl(char c)
        {
                return (c == '\n') || (c == '\r');
        }

        bool archive_stream_t::getline(std::string& line)
        {
                /// \todo not very efficient: should buffer larger chunks (1K ?!) and check for endline there!
                char c;
                while (read(&c, 1) && isendl(c)) {}

                line.clear();
                while (read(&c, 1) && !isendl(c)) { line.push_back(c); }

                return !line.empty();
        }

        std::streamsize archive_stream_t::size() const
        {
                const void* buff = nullptr;
                size_t size = 0;

                std::streamsize num_bytes = 0;
                while (advance(buff, size))
                {
                        num_bytes += size;
                }

                return num_bytes;
        }

        bool archive_stream_t::advance(const void*& buffer, size_t& size) const
        {
                off_t offset;
                const int r = archive_read_data_block(m_archive, &buffer, &size, &offset);
                return r == ARCHIVE_OK;
        }

        static bool decode(archive* ar,
                const archive_callback_t& callback,
                const archive_error_callback_t& error_callback)
        {
                bool ok = true;
                while (ok)
                {
                        archive_entry* entry;
                        const int r = archive_read_next_header(ar, &entry);

                        if (r == ARCHIVE_EOF)
                                break;
                        if (r != ARCHIVE_OK)
                        {
                                error_callback("failed to read archive!");
                                error_callback(std::string("error <") + archive_error_string(ar) + ">!");
                                ok = false;
                                break;
                        }

                        const std::string filename = archive_entry_pathname(entry);

                        archive_stream_t stream(ar);
                        ok = callback(filename, stream);
                }

                archive_read_close(ar);
                archive_read_free(ar);

                // OK
                return ok;
        }

        bool unarchive(const std::string& path,
                const archive_callback_t& callback,
                const archive_error_callback_t& error_callback)
        {
                archive* ar = archive_read_new();

                archive_read_support_filter_all(ar);
                archive_read_support_format_all(ar);
                archive_read_support_format_raw(ar);

                if (archive_read_open_filename(ar, path.c_str(), 10240))
                {
                        error_callback("failed to open archive <" + path + ">!");
                        error_callback(std::string("error <") + archive_error_string(ar) + ">!");
                        return false;
                }

                return decode(ar, callback, error_callback);
        }
}
