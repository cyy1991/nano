#pragma once

#include "conv2d_assert.hpp"

namespace tensor
{
        ///
        /// \brief 2D correlation: idata += odata @ kdata (using a mad product by odata columns)
        ///
        struct corr2d_mdo_t
        {
                template
                <
                        typename tmatrixo,
                        typename tmatrixk,
                        typename tmatrixi
                >
                void operator()(const tmatrixo& odata, const tmatrixk& kdata, tmatrixi&& idata) const
                {
                        conv2d_assert(idata, kdata, odata);

                        const auto orows = odata.rows();
                        const auto ocols = odata.cols();
                        const auto krows = kdata.rows();
                        const auto kcols = kdata.cols();

                        for (auto r = 0; r < orows; r ++)
                        {
                                for (auto kr = 0; kr < krows; kr ++)
                                {
                                        const auto orow = odata.row(r);

                                        for (auto kc = 0; kc < kcols; kc ++)
                                        {
                                                idata.row(r + kr).segment(kc, ocols) +=
                                                orow * kdata(kr, kc);
                                        }
                                }
                        }
                }
        };

        ///
        /// \brief 2D correlation: idata += odata @ kdata (using a fixed-size mad product by odata columns)
        ///
        template
        <
                int ocols
        >
        struct corr2d_mdoi_t
        {
                template
                <
                        typename tmatrixo,
                        typename tmatrixk,
                        typename tmatrixi
                >
                void operator()(const tmatrixo& odata, const tmatrixk& kdata, tmatrixi&& idata) const
                {
                        conv2d_assert(idata, kdata, odata);
                        assert(odata.cols() == ocols);

                        const auto orows = odata.rows();
                        const auto krows = kdata.rows();
                        const auto kcols = kdata.cols();

                        for (auto r = 0; r < orows; r ++)
                        {
                                for (auto kr = 0; kr < krows; kr ++)
                                {
                                        const auto orow = odata.row(r);

                                        for (auto kc = 0; kc < kcols; kc ++)
                                        {
                                                idata.row(r + kr).template segment<ocols>(kc) +=
                                                orow * kdata(kr, kc);
                                        }
                                }
                        }
                }
        };

        ///
        /// \brief 2D correlation: idata += odata @ kdata (using a mad product by odata runtime-decoded columns)
        ///
        struct corr2d_mdo_dyn_t
        {
                template
                <
                        typename tmatrixo,
                        typename tmatrixk,
                        typename tmatrixi
                >
                void operator()(const tmatrixo& odata, const tmatrixk& kdata, tmatrixi&& idata) const
                {
                        switch (odata.cols())
                        {
                        case 1:         corr2d_mdoi_t<1>()(odata, kdata, idata); break;
                        case 2:         corr2d_mdoi_t<2>()(odata, kdata, idata); break;
                        case 3:         corr2d_mdoi_t<3>()(odata, kdata, idata); break;
                        case 4:         corr2d_mdoi_t<4>()(odata, kdata, idata); break;
                        case 5:         corr2d_mdoi_t<5>()(odata, kdata, idata); break;
                        case 6:         corr2d_mdoi_t<6>()(odata, kdata, idata); break;
                        case 7:         corr2d_mdoi_t<7>()(odata, kdata, idata); break;
                        case 8:         corr2d_mdoi_t<8>()(odata, kdata, idata); break;
                        case 9:         corr2d_mdoi_t<9>()(odata, kdata, idata); break;
                        case 10:        corr2d_mdoi_t<10>()(odata, kdata, idata); break;
                        case 11:        corr2d_mdoi_t<11>()(odata, kdata, idata); break;
                        case 12:        corr2d_mdoi_t<12>()(odata, kdata, idata); break;
                        case 13:        corr2d_mdoi_t<13>()(odata, kdata, idata); break;
                        case 14:        corr2d_mdoi_t<14>()(odata, kdata, idata); break;
                        case 15:        corr2d_mdoi_t<15>()(odata, kdata, idata); break;
                        default:        corr2d_mdo_t()(odata, kdata, idata); break;
                        }
                }
        };
}

