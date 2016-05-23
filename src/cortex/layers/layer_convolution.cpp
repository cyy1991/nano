#include "math/clamp.hpp"
#include "math/random.hpp"
#include "cortex/logger.h"
#include "tensor/numeric.hpp"
#include "text/to_string.hpp"
#include "layer_convolution.h"
#include "text/from_params.hpp"
#include "tensor/serialize.hpp"

namespace nano
{
        conv_layer_t::conv_layer_t(const string_t& parameters) :
                layer_t(parameters),
                m_impl(1, 1, 1, 1, 1, 1, 1)
        {
        }

        tensor_size_t conv_layer_t::resize(const tensor3d_t& tensor)
        {
                const auto idims = tensor.size<0>();
                const auto irows = tensor.size<1>();
                const auto icols = tensor.size<2>();

                const auto odims = nano::clamp(nano::from_params<tensor_size_t>(configuration(), "dims", 16), 1, 256);
                const auto krows = nano::clamp(nano::from_params<tensor_size_t>(configuration(), "rows", 8), 1, 32);
                const auto kcols = nano::clamp(nano::from_params<tensor_size_t>(configuration(), "cols", 8), 1, 32);
                const auto kconn = tensor_size_t(1);
                //nano::clamp(nano::from_params<tensor_size_t>(configuration(), "conn", 1), 1, 16);

                const auto orows = irows - krows + 1;
                const auto ocols = icols - kcols + 1;

                // check convolution size
                if (irows < krows || icols < kcols)
                {
                        log_error() << "convolution layer: invalid size (" << idims << "x" << irows << "x" << icols
                                    << ") -> (" << odims << "x" << krows << "x" << kcols << ")!";
                        throw std::invalid_argument("invalid configuration for the convolution layer");
                }

                // check input connectivity factor
                if ((idims % kconn) || (odims % kconn))
                {
                        log_error() << "convolution layer: invalid input connectivity factor!";
                        throw std::invalid_argument("invalid configuration for the convolution layer");
                }

                // resize buffers
                m_idata.resize(idims, irows, icols);
                m_odata.resize(odims, orows, ocols);
                m_kdata.resize(odims, idims / kconn, krows, kcols);
                m_bdata.resize(odims, 1, 1);
                m_impl = impl_t(idims, irows, icols, odims, krows, kcols, kconn);

                return psize();
        }

        void conv_layer_t::zero_params()
        {
                tensor::set_zero(m_kdata, m_bdata);
        }

        void conv_layer_t::random_params(scalar_t min, scalar_t max)
        {
                tensor::set_random(nano::random_t<scalar_t>(min, max), m_kdata, m_bdata);
        }

        scalar_t* conv_layer_t::save_params(scalar_t* params) const
        {
                return tensor::to_array(params, m_kdata, m_bdata);
        }

        const scalar_t* conv_layer_t::load_params(const scalar_t* params)
        {
                return tensor::from_array(params, m_kdata, m_bdata);
        }

        const tensor3d_t& conv_layer_t::output(const tensor3d_t& input)
        {
                assert(idims() == input.size<0>());
                assert(irows() == input.size<1>());
                assert(icols() == input.size<2>());

                m_idata = input;

                // convolution
                m_impl.output(m_idata, m_kdata, m_odata);

                // +bias
                for (tensor_size_t o = 0; o < odims(); ++ o)
                {
                        m_odata.vector(o).array() += m_bdata(o);
                }

                return m_odata;
        }

        const tensor3d_t& conv_layer_t::ginput(const tensor3d_t& output)
        {
                assert(odims() == output.size<0>());
                assert(orows() == output.size<1>());
                assert(ocols() == output.size<2>());

                m_odata = output;

                m_impl.ginput(m_idata, m_kdata, m_odata);

                return m_idata;
        }

        void conv_layer_t::gparam(const tensor3d_t& output, scalar_t* gradient)
        {
                assert(odims() == output.size<0>());
                assert(orows() == output.size<1>());
                assert(ocols() == output.size<2>());

                m_odata = output;

                // wrt convolution
                auto gkdata = tensor::map_tensor(gradient, m_kdata.size<0>(), m_kdata.size<1>(), krows(), kcols());
                m_impl.gparam(m_idata, gkdata, m_odata);

                // wrt bias
                for (tensor_size_t o = 0; o < odims(); ++ o)
                {
                        gradient[m_kdata.size() + o] = m_odata.vector(o).sum();
                }
        }
}


