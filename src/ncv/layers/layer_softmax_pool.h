#ifndef NANOCV_SOFTMAX_POOL_LAYER_H
#define NANOCV_SOFTMAX_POOL_LAYER_H

#include "layer.h"

namespace ncv
{
        ///
        /// softmax pooling layer:
        ///      down-sample by 2 from a 3x3 neighbouring region using a soft-max weighting.
        ///      weight ~ input value.
        ///
        class softmax_pool_layer_t : public layer_t
        {
        public:

                // constructor
                softmax_pool_layer_t(const string_t& params = string_t());

                NCV_MAKE_CLONABLE(softmax_pool_layer_t, layer_t,
                                  "soft-max pooling layer")

                // resize to process new tensors of the given type
                virtual size_t resize(const tensor_t& tensor);

                // reset parameters
                virtual void zero_params() {}
                virtual void random_params(scalar_t min, scalar_t max) {}

                // serialize parameters & gradients
                virtual ovectorizer_t& save_params(ovectorizer_t& s) const { return s; }
                virtual ovectorizer_t& save_grad(ovectorizer_t& s) const { return s; }
                virtual ivectorizer_t& load_params(ivectorizer_t& s) { return s; }

                // process inputs (compute outputs & gradients)
                virtual const tensor_t& forward(const tensor_t& input);
                virtual const tensor_t& backward(const tensor_t& gradient);

                // save/load parameters to/from file
                virtual bool save(boost::archive::binary_oarchive& oa) const { return true; }
                virtual bool load(boost::archive::binary_iarchive& ia) { return true; }

                // access functions
                virtual const tensor_t& input() const { return m_idata; }
                virtual const tensor_t& output() const { return m_odata; }

        private:

                size_t idims() const { return m_idata.dims(); }
                size_t irows() const { return m_idata.rows(); }
                size_t icols() const { return m_idata.cols(); }

                size_t odims() const { return m_odata.dims(); }
                size_t orows() const { return m_odata.rows(); }
                size_t ocols() const { return m_odata.cols(); }

        private:

                // attributes
                tensor_t                m_idata;        ///< input buffer
                tensor_t                m_odata;        ///< output buffer

                tensor_t                m_wdata;        ///< pooling weights
                tensor_t                m_sdata;        ///< nominator
                tensor_t                m_tdata;        ///< denominator
        };
}

#endif // NANOCV_SOFTMAX_POOL_LAYER_H

