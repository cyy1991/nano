#ifndef NANOCV_LAYER_H
#define NANOCV_LAYER_H

#include "common/manager.hpp"
#include "types.h"

namespace ncv
{
        class layer_t;

        ///
        /// \brief stores registered prototypes
        ///
        typedef manager_t<layer_t>                      layer_manager_t;
        typedef layer_manager_t::robject_t              rlayer_t;
        typedef std::vector<rlayer_t>                   rlayers_t;

        ///
        /// \brief process a set of inputs of size (irows, icols) and produces a set of outputs of size (orows, ocols)
        ///
        class layer_t : public clonable_t<layer_t>
        {
        public:

                ///
                /// \brief constructor
                ///
                layer_t(const string_t& parameters, const string_t& description)
                        :       clonable_t<layer_t>(parameters, description),
                                m_enabled(true)
                {
                }

                ///
                /// \brief destructor
                ///
                virtual ~layer_t() {}

                ///
                /// \brief resize to process new tensors of the given type
                ///
                virtual size_t resize(const tensor_t& tensor) = 0;

                ///
                /// \brief reset parameters to zero
                ///
                virtual void zero_params() = 0;

                ///
                /// \brief reset parameters to random values in the [min, max] range
                ///
                virtual void random_params(scalar_t min, scalar_t max) = 0;

                ///
                /// \brief serialize parameters
                ///
                virtual scalar_t* save_params(scalar_t* params) const = 0;
                virtual const scalar_t* load_params(const scalar_t* params) = 0;

                ///
                /// \brief compute the output/gradient tensor
                ///
                virtual const tensor_t& forward(const tensor_t& input) = 0;
                virtual const tensor_t& backward(const tensor_t& output, scalar_t* gradient) = 0;

                ///
                /// \brief toggle the layer (for gradient computation)
                ///
                bool enable() { return toggable() && (m_enabled = true); }
                bool disable() { return toggable() && !(m_enabled = false); }

                ///
                /// \brief returns the input/output dimensions
                ///
                virtual size_t idims() const = 0;
                virtual size_t irows() const = 0;
                virtual size_t icols() const = 0;

                virtual size_t odims() const = 0;
                virtual size_t orows() const = 0;
                virtual size_t ocols() const = 0;

                ///
                /// \brief returns the number of (optimization) parameters
                ///
                virtual size_t psize() const = 0;

                ///
                /// \brief returns the toggling state (for gradient computation)
                ///
                bool enabled() const { return m_enabled; }
                bool toggable() const { return psize() > 0; }

        private:

                // attributes
                bool            m_enabled;
        };
}

#endif // NANOCV_LAYER_H
