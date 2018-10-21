#pragma once

#include "index.h"

namespace nano
{
        template <typename tscalar_>
        class tensor_pstorage_t;

        ///
        /// \brief tensor storage using an Eigen vector.
        /// NB: the tensor owns the allocated memory and as such the tensor is resizable.
        ///
        template <typename tscalar_>
        class tensor_vstorage_t
        {
        public:
                using tscalar = typename std::remove_const<tscalar_>::type;
                using tstorage = tensor_vector_t<tscalar>;
                using treference = tscalar&;
                using tconst_reference = const tscalar&;

                static constexpr bool resizable = true;
                static constexpr bool owns_memory = true;

                tensor_vstorage_t() = default;
                tensor_vstorage_t(tensor_vstorage_t&&) = default;
                tensor_vstorage_t(const tensor_vstorage_t&) = default;
                tensor_vstorage_t& operator=(tensor_vstorage_t&&) = default;
                tensor_vstorage_t& operator=(const tensor_vstorage_t&) = default;

                template <typename tscalar2_>
                tensor_vstorage_t(const tensor_pstorage_t<tscalar2_>&);

                explicit tensor_vstorage_t(const tstorage& data) : m_data(data) {}
                explicit tensor_vstorage_t(const tensor_size_t size) : m_data(size) {}

                auto size() const { return m_data.size(); }
                void resize(const tensor_size_t size) { m_data.resize(size); }

                auto data() { return m_data.data(); }
                auto data() const { return m_data.data(); }

        private:

                // attributes
                tstorage        m_data;         ///< store tensor as a 1D vector.
        };

        ///
        /// \brief tensor storage using a C-array.
        /// NB: the tensors doesn't own the allocated memory and as such is not resizable.
        ///
        template <typename tscalar_>
        class tensor_pstorage_t
        {
        public:
                using tscalar = typename std::remove_const<tscalar_>::type;
                using tstorage = tscalar_*;
                using treference = typename std::conditional<std::is_const<tscalar_>::value, const tscalar&, tscalar&>::type;
                using tconst_reference = treference;

                static constexpr bool resizable = false;
                static constexpr bool owns_memory = false;

                tensor_pstorage_t(const tstorage& data, const tensor_size_t size) :
                        m_data(data), m_size(size)
                {
                }

                template <typename tscalar2_>
                tensor_pstorage_t(const tensor_vstorage_t<tscalar2_>& other) :
                        m_data(other.data()),
                        m_size(other.size())
                {
                }

                template <typename tscalar2_>
                tensor_pstorage_t(const tensor_pstorage_t<tscalar2_>& other) :
                        m_data(other.data()),
                        m_size(other.size())
                {
                }

                template <typename tscalar2_>
                tensor_pstorage_t& operator=(const tensor_pstorage_t<tscalar2_>& other)
                {
                        assert(size() == other.size());
                        map_vector(data(), size()) = map_vector(other.data(), other.size());
                        return *this;
                }

                auto data() { return m_data; }
                auto data() const { return m_data; }
                auto size() const { return m_size; }

        private:

                // attributes
                tstorage        m_data{nullptr};///< wrap tensor over a contiguous array.
                tensor_size_t   m_size{0};      ///<
        };

        template <typename tscalar_>
        template <typename tscalar2_>
        tensor_vstorage_t<tscalar_>::tensor_vstorage_t(const tensor_pstorage_t<tscalar2_>& other) :
                m_data(map_vector(other.data(), other.size()))
        {
        }
}
