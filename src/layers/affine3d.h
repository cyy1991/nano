#pragma once

#include "affine_params.h"

namespace nano
{
        ///
        /// \brief affine transformation with 4D input and output tensors using
        ///     level-2 Blas calls (thus processing each sample independently).
        ///
        /// parameters:
        ///     idata: 4D input tensor (count x imaps x irows x icols, with isize = imaps x irows x icols)
        ///     wdata: weight matrix (osize x isize)
        ///     bdata: bias vector (osize)
        ///     odata: 4D output tensor (count x omaps x orows x ocols, with osize = omaps x orows x ocols)
        ///
        struct affine3d_t
        {
                ///
                /// \brief constructor
                ///
                explicit affine3d_t(const affine_params_t& params = affine_params_t()) :
                        m_params(params) {}

                ///
                /// \brief output
                ///
                template <typename tidata, typename twdata, typename tbdata, typename todata>
                bool output(const tidata& idata, const twdata& wdata, const tbdata& bdata, todata&& odata) const;

                ///
                /// \brief gradient wrt inputs
                ///
                template <typename tidata, typename twdata, typename tbdata, typename todata>
                bool ginput(tidata&& idata, const twdata& wdata, const tbdata& bdata, const todata& odata) const;

                ///
                /// \brief accumulate the gradient wrt parameters (weights and bias)
                ///
                template <typename tidata, typename twdata, typename tbdata, typename todata>
                bool gparam(const tidata& idata, twdata&& wdata, tbdata&& bdata, const todata& odata) const;

                ///
                /// \brief parameters
                ///
                const affine_params_t& params() const { return m_params; }

        private:

                // attributes
                affine_params_t         m_params;
        };

        template <typename tidata, typename twdata, typename tbdata, typename todata>
        bool affine3d_t::output(const tidata& idata, const twdata& wdata, const tbdata& bdata, todata&& odata) const
        {
                if (!m_params.valid(idata, wdata, bdata, odata))
                {
                        return false;
                }

                const auto count = idata.template size<0>();
                const auto isize = m_params.isize();
                const auto osize = m_params.osize();

                for (tensor_size_t x = 0; x < count; ++ x)
                {
                        map_vector(odata.data() + x * osize, osize) = wdata *
                        map_vector(idata.data() + x * isize, isize) + bdata;
                }
                return true;
        }

        template <typename tidata, typename twdata, typename tbdata, typename todata>
        bool affine3d_t::ginput(tidata&& idata, const twdata& wdata, const tbdata& bdata, const todata& odata) const
        {
                if (!m_params.valid(idata, wdata, bdata, odata))
                {
                        return false;
                }

                const auto count = idata.template size<0>();
                const auto isize = m_params.isize();
                const auto osize = m_params.osize();

                for (tensor_size_t x = 0; x < count; ++ x)
                {
                        map_vector(idata.data() + x * isize, isize) = wdata.transpose() *
                        map_vector(odata.data() + x * osize, osize);
                }
                return true;
        }

        template <typename tidata, typename twdata, typename tbdata, typename todata>
        bool affine3d_t::gparam(const tidata& idata, twdata&& wdata, tbdata&& bdata, const todata& odata) const
        {
                if (!m_params.valid(idata, wdata, bdata, odata))
                {
                        return false;
                }

                const auto count = idata.template size<0>();
                const auto isize = m_params.isize();
                const auto osize = m_params.osize();

                wdata.setZero();
                bdata.setZero();
                for (tensor_size_t x = 0; x < count; ++ x)
                {
                        wdata += map_vector(odata.data() + x * osize, osize) *
                                 map_vector(idata.data() + x * isize, isize).transpose();
                        bdata += map_vector(odata.data() + x * osize, osize);
                }
                return true;
        }
}