#pragma once

namespace nano
{
        template <typename tscalar, typename tvalue>
        class average_t
        {
        public:
                explicit average_t(const tvalue& initial) :
                        m_count(0),
                        m_value(initial)
                {
                }

                void update(const tvalue& value)
                {
                        m_count ++;
                        m_value = (m_value * (m_count - 1) + value) * (1 / m_count);
                }

                const auto& value() const
                {
                        return m_value;
                }

        private:

                tscalar         m_count;
                tvalue          m_value;        ///< running average
        };

        ///
        /// \brief running average for scalars
        ///
        template <typename tscalar, typename tbase = average_t<tscalar, tscalar>>
        struct average_scalar_t : public tbase
        {
                average_scalar_t() :
                        tbase(0)
                {
                }
        };

        ///
        /// \brief running average for Eigen vectors
        ///
        template <typename tvector, typename tbase = average_t<typename tvector::Scalar, tvector>>
        struct average_vector_t : public tbase
        {
                template <typename tsize>
                explicit average_vector_t(const tsize dimensions) :
                        tbase(tvector::Zero(dimensions))
                {
                }
        };
}

