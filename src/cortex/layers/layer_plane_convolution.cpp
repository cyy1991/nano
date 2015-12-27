#include "layer_plane_convolution.h"
#include "math/clamp.hpp"
#include "math/random.hpp"
#include "tensor/random.hpp"
#include "tensor/conv3d.hpp"
#include "text/to_string.hpp"
#include "cortex/util/logger.h"
#include "text/from_params.hpp"
#include "tensor/serialize.hpp"
#include "tensor/conv2d_dyn.hpp"
#include "tensor/corr2d_dyn.hpp"

namespace cortex
{
        plane_conv_layer_t::plane_conv_layer_t(const string_t& parameters)
                :       layer_t(parameters)
        {
        }

        plane_conv_layer_t::~plane_conv_layer_t()
        {
        }

        tensor_size_t plane_conv_layer_t::resize(const tensor_t& tensor)
        {
                const auto idims = tensor.dims();
                const auto irows = tensor.rows();
                const auto icols = tensor.cols();

                const auto kdims = math::clamp(text::from_params<tensor_size_t>(configuration(), "dims", 16), 1, 256);
                const auto krows = math::clamp(text::from_params<tensor_size_t>(configuration(), "rows", 8), 1, 32);
                const auto kcols = math::clamp(text::from_params<tensor_size_t>(configuration(), "cols", 8), 1, 32);

                const auto odims = idims * kdims;
                const auto orows = irows - krows + 1;
                const auto ocols = icols - kcols + 1;

                // check convolution size
                if (irows < krows || icols < kcols)
                {
                        const string_t message =
                                "invalid size (" + text::to_string(idims) + "x" + text::to_string(irows) +
                                 "x" + text::to_string(icols) + ") -> (" + text::to_string(odims) + "x" +
                                 text::to_string(krows) + "x" + text::to_string(kcols) + ")";

                        log_error() << "plane-based convolution layer: " << message;
                        throw std::runtime_error("plane-based convolution layer: " + message);
                }

                // resize buffers
                m_idata.resize(idims, irows, icols);
                m_odata.resize(odims, orows, ocols);
                m_kdata.resize(kdims, krows, kcols);

                return psize();
        }

        void plane_conv_layer_t::zero_params()
        {
                m_kdata.setZero();
        }

        void plane_conv_layer_t::random_params(scalar_t min, scalar_t max)
        {
                tensor::set_random(m_kdata, math::random_t<scalar_t>(min, max));
        }

        scalar_t* plane_conv_layer_t::save_params(scalar_t* params) const
        {
                params = tensor::to_array(m_kdata, params);
                return params;
        }

        const scalar_t* plane_conv_layer_t::load_params(const scalar_t* params)
        {
                params = tensor::from_array(m_kdata, params);

                return params;
        }

        tensor_size_t plane_conv_layer_t::psize() const
        {
                return m_kdata.size();
        }

        const tensor_t& plane_conv_layer_t::output(const tensor_t& input)
        {
                assert(idims() == input.dims());
                assert(irows() == input.rows());
                assert(icols() == input.cols());

                m_idata = input;

                m_odata.setZero();

                tensor::conv2d_dyn_t op;
                for (tensor_size_t i = 0, o = 0; i < idims(); ++ i)
                {
                        for (tensor_size_t k = 0; k < kdims(); ++ k, ++ o)
                        {
                                op(m_idata.matrix(i), m_kdata.matrix(k), m_odata.matrix(o));
                        }
                }

                return m_odata;
        }        

        const tensor_t& plane_conv_layer_t::ginput(const tensor_t& output)
        {
                assert(odims() == output.dims());
                assert(orows() == output.rows());
                assert(ocols() == output.cols());

                m_odata = output;

                m_idata.setZero();

                tensor::corr2d_dyn_t op;
                for (tensor_size_t i = 0, o = 0; i < idims(); ++ i)
                {
                        for (tensor_size_t k = 0; k < kdims(); ++ k, ++ o)
                        {
                                op(m_kdata.matrix(k), m_odata.matrix(o), m_idata.matrix(i));
                        }
                }

                return m_idata;
        }

        void plane_conv_layer_t::gparam(const tensor_t& output, scalar_t* gradient)
        {
                assert(odims() == output.dims());
                assert(orows() == output.rows());
                assert(ocols() == output.cols());

                m_odata = output;
                
                auto gkdata = tensor::map_tensor(gradient, kdims(), krows(), kcols());
                gkdata.setZero();

                tensor::conv2d_dyn_t op;
                for (tensor_size_t i = 0, o = 0; i < idims(); ++ i)
                {
                        for (tensor_size_t k = 0; k < kdims(); ++ k, ++ o)
                        {
                                op(m_idata.matrix(i), m_odata.matrix(o), gkdata.matrix(k));
                        }
                }
        }
}


