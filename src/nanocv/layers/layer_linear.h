#pragma once

#include "layer.h"

namespace ncv
{
        ///
        /// \brief fully-connected linear layer (as in MLP models)
        ///
        /// parameters:
        ///     dims=10[1,4096]          - number of output dimensions
        ///
        class linear_layer_t : public layer_t
        {
        public:

                NANOCV_MAKE_CLONABLE(linear_layer_t, "fully-connected linear layer, parameters: dims=10[1,4096]")

                // constructor
                linear_layer_t(const string_t& parameters = string_t());

                // resize to process new tensors of the given type
                virtual size_t resize(const tensor_t& tensor);

                // reset parameters
                virtual void zero_params();
                virtual void random_params(scalar_t min, scalar_t max);

                // serialize parameters
                virtual scalar_t* save_params(scalar_t* params) const;
                virtual const scalar_t* load_params(const scalar_t* params);

                // serialize parameters (to disk)
                virtual boost::archive::binary_oarchive& save(boost::archive::binary_oarchive& oa) const;
                virtual boost::archive::binary_iarchive& load(boost::archive::binary_iarchive& ia);

                // process inputs (compute outputs & gradients)
                virtual const tensor_t& output(const tensor_t& input);
                virtual const tensor_t& igrad(const tensor_t& output);
                virtual void pgrad(const tensor_t& output, scalar_t* gradient);

                // access functions
                virtual size_t idims() const { return m_idata.dims(); }
                virtual size_t irows() const { return m_idata.rows(); }
                virtual size_t icols() const { return m_idata.cols(); }
                virtual size_t odims() const { return m_odata.dims(); }
                virtual size_t orows() const { return m_odata.rows(); }
                virtual size_t ocols() const { return m_odata.cols(); }
                virtual size_t psize() const { return m_wdata.size() + m_bdata.size(); }

                // flops
                virtual size_t output_flops() const { return osize() + osize() * isize(); }
                virtual size_t igrad_flops() const { return osize() * isize(); }
                virtual size_t pgrad_flops() const { return osize() * isize() + osize(); }

        private:

                size_t isize() const { return m_idata.size(); }
                size_t osize() const { return m_odata.size(); }

        private:

                // attributes
                tensor_t                m_idata;        ///< input buffer:      isize x 1 x 1
                tensor_t                m_odata;        ///< output buffer:     osize x 1 x 1

                tensor_t                m_wdata;        ///< weights:           1 x osize x isize
                tensor_t                m_bdata;        ///< bias:              osize x 1 x 1
        };
}
