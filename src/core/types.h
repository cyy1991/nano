#ifndef NANOCV_TYPES_H
#define NANOCV_TYPES_H

#include "tensor/tensor3d.hpp"
#include "tensor/tensor4d.hpp"
#include "optimize/algo.hpp"
#include <functional>
#include <string>
#include <cstdint>

namespace ncv
{
        // numerical types
        typedef std::size_t                                     size_t;
        typedef std::vector<size_t>                             indices_t;

        typedef double                                          scalar_t;
        typedef std::vector<scalar_t>                           scalars_t;

        typedef tensor::vector_types_t<scalar_t>::vector_t      vector_t;
        typedef tensor::vector_types_t<scalar_t>::vectors_t     vectors_t;

        typedef tensor::matrix_types_t<scalar_t>::matrix_t      matrix_t;
        typedef tensor::matrix_types_t<scalar_t>::matrices_t    matrices_t;

        typedef tensor::tensor3d_t<scalar_t, size_t>            tensor3d_t;
        typedef std::vector<tensor3d_t>                         tensor3ds_t;

        typedef tensor::tensor4d_t<scalar_t, size_t>            tensor4d_t;
        typedef std::vector<tensor4d_t>                         tensor4ds_t;

        // strings
        typedef std::string                                     string_t;
        typedef std::vector<string_t>                           strings_t;

        // lambda
        using std::placeholders::_1;
        using std::placeholders::_2;
        using std::placeholders::_3;
        using std::placeholders::_4;

        // alignment options
        enum class align : int
        {
                left,
                center,
                right
        };

        // color channels
        enum class channel : int
        {
                red = 0,                // R
                green,                  // G
                blue,                   // B
                luma,                   // Y/L
                cielab_l,               // CIELab L
                cielab_a,               // CIELab a
                cielab_b                // CIELab b
        };

        // machine learning protocol
        enum class protocol : int
        {
                train = 0,              // training
                test                    // testing
        };

        // color processing mode method
        enum class color_mode : int
        {
                luma,                   // process only grayscale color channel
                rgba                    // process red, green & blue color channels
        };

        // optimizer & result
        typedef optimize::optimizer<scalar_t, size_t>   optimizer_t;
        typedef optimizer_t::tproblem                   optproblem_t;
        typedef optimizer_t::tresult                    optresult_t;
}

#endif // NANOCV_TYPES_H

