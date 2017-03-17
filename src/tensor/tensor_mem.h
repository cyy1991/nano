#pragma once

#include "tensor_map.h"

namespace nano
{
        ///
        /// \brief tensor that owns the allocated memory.
        ///
        template <typename tscalar, std::size_t tdimensions>
        struct tensor_mem_t : public tensor_indexer_t<tdimensions>
        {
                using tbase = tensor_indexer_t<tdimensions>;

                using tvector = tensor_vector_t<tscalar>;
                using tdims = typename tbase::tdims;
                using Index = typename tbase::Index;
                using Scalar = tscalar;

                ///
                /// \brief constructor
                ///
                tensor_mem_t() = default;

                ///
                /// \brief constructor
                ///
                template <typename... tsizes>
                explicit tensor_mem_t(const tsizes... dims) :
                        tbase(dims...)
                {
                        m_data.resize(this->size());
                }

                ///
                /// \brief constructor
                ///
                explicit tensor_mem_t(const tdims& dims) :
                        tbase(dims)
                {
                        m_data.resize(this->size());
                }

                ///
                /// \brief constructor
                ///
                template <typename tscalar2>
                tensor_mem_t(const tensor_array_t<tscalar2, tdimensions>& other) :
                        tbase(other.dims())
                {
                        m_data = other.vector();
                }

                ///
                /// \brief resize to new dimensions
                ///
                template <typename... tsizes>
                tensor_index_t resize(const tsizes... dims)
                {
                        return resize({{dims...}});
                }

                ///
                /// \brief resize to new dimensions
                ///
                tensor_index_t resize(const tdims& dims)
                {
                        this->m_dims = dims;
                        m_data.resize(nano::size(this->m_dims));
                        return this->size();
                }

                ///
                /// \brief access the whole tensor as a vector (size() x 1)
                ///
                auto vector() const { return map_vector(data(), this->size()); }
                auto vector() { return map_vector(data(), this->size()); }

                ///
                /// \brief access the whole tensor as an array (size() x 1)
                ///
                auto array() const { return vector().array(); }
                auto array() { return vector().array(); }

                ///
                /// \brief access the whole tensor as a C-array
                ///
                auto data() const { return m_data.data(); }
                auto data() { return m_data.data(); }

                ///
                /// \brief access the 2D plane (indices...) as a vector
                ///
                template <typename... tindices>
                auto vector(const tindices... indices) const
                {
                        return map_vector(planeData(indices...), this->planeSize());
                }
                template <typename... tindices>
                auto vector(const tindices... indices)
                {
                        return map_vector(planeData(indices...), this->planeSize());
                }

                ///
                /// \brief access the 2D plane (indices...) as an array
                ///
                template <typename... tindices>
                auto array(const tindices... indices) const
                {
                        return vector(indices...).array();
                }
                template <typename... tindices>
                auto array(const tindices... indices)
                {
                        return vector(indices...).array();
                }

                ///
                /// \brief access the 2D plane (indices...) as matrix
                ///
                template <typename... tindices>
                auto matrix(const tindices... indices) const
                {
                        return map_matrix(planeData(indices...), this->rows(), this->cols());
                }
                template <typename... tindices>
                auto matrix(const tindices... indices)
                {
                        return map_matrix(planeData(indices...), this->rows(), this->cols());
                }

                ///
                /// \brief access the 2D plane (indices...) as a C-array
                ///
                template <typename... tindices>
                auto planeData(const tindices... indices) const
                {
                        static_assert(sizeof...(indices) == tdimensions - 2, "method not available");
                        return data() + nano::index(this->dims(), indices..., 0, 0);
                }
                template <typename... tindices>
                auto planeData(const tindices... indices)
                {
                        static_assert(sizeof...(indices) == tdimensions - 2, "method not available");
                        return data() + nano::index(this->dims(), indices..., 0, 0);
                }

                ///
                /// \brief access an element of the tensor
                ///
                const tscalar& operator()(const tensor_index_t index) const
                {
                        return m_data(index);
                }
                tscalar& operator()(const tensor_index_t index)
                {
                        return m_data(index);
                }

                ///
                /// \brief access an element of the tensor
                ///
                template <typename... tindices>
                const tscalar& operator()(const tensor_index_t index, const tindices... indices) const
                {
                        return m_data(nano::index(this->dims(), index, indices...));
                }
                template <typename... tindices>
                tscalar& operator()(const tensor_index_t index, const tindices... indices)
                {
                        return m_data(nano::index(this->dims(), index, indices...));
                }

        private:

                // attributes
                tvector         m_data;         ///< tensor stored as 1D vector
        };
}