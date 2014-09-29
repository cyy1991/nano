#pragma once

#include "color.h"
#include "geom.h"

namespace ncv
{
        ///
        /// \brief stores an image either as grayscale (luma) or RGBA buffer.
        ///
        /// operations:
        ///     - loading and saving from and to files
        ///     - scaling to [0, 1] tensors
        ///
        /// fixme: merge grid_image here!
        ///
        class image_t
        {
        public:

                ///
                /// \brief constructor
                ///
                image_t(coord_t rows = 0, coord_t cols = 0, color_mode mode = color_mode::rgba);

                ///
                /// \brief resize to new dimensions
                ///
                void resize(coord_t rows, coord_t cols, color_mode mode);

                ///
                /// \brief load image from disk
                ///
                bool load_rgba(const string_t& path);
                bool load_luma(const string_t& path);

                ///
                /// \brief load image from buffer
                ///
                bool load_luma(const char* buffer, coord_t rows, coord_t cols);
                bool load_rgba(const char* buffer, coord_t rows, coord_t cols);
                bool load_rgba(const char* buffer, coord_t rows, coord_t cols, coord_t stride);
                bool load_rgba(const rgba_matrix_t& data);
                bool load_luma(const rgba_matrix_t& data);
                bool load_luma(const luma_matrix_t& data);

                ///
                /// \brief load image from scaled [0, 1] tensor
                ///     having 1 (lumascale) or 3 (rgba) planes
                ///
                bool load(const tensor_t& data);

                ///
                /// \brief save image to disk
                ///
                bool save(const string_t& path) const;

                ///
                /// \brief save image to scaled [0, 1] tensor
                ///     with 1 (luma) or 3 (rgba) planes
                ///
                tensor_t to_tensor() const;
                tensor_t to_tensor(const rect_t& region) const;

                ///
                /// \brief transform between color mode
                ///
                bool make_rgba();
                bool make_luma();

                ///
                /// \brief fill with constant color
                ///
                bool fill(rgba_t rgba);
                bool fill(luma_t luma);

                ///
                /// \brief copy the given (region of the given) patch at the (r, c) location
                ///
                bool copy(coord_t top, coord_t left, const rgba_matrix_t& patch);
                bool copy(coord_t top, coord_t left, const luma_matrix_t& patch);

                bool copy(coord_t top, coord_t left, const image_t& patch);
                bool copy(coord_t top, coord_t left, const image_t& patch, const rect_t& region);

                ///
                /// \brief change a pixel
                ///
                bool set(coord_t row, coord_t col, rgba_t rgba);
                bool set(coord_t row, coord_t col, luma_t luma);

                ///
                /// \brief transpose in place the pixel matrix
                ///
                void transpose_in_place();

                ///
                /// \brief scale with the given factor
                ///
                bool scale(scalar_t factor);

                // access functions
                coord_t rows() const { return m_rows; }
                coord_t cols() const { return m_cols; }
                color_mode mode() const { return m_mode; }

                bool is_rgba() const { return mode() == color_mode::rgba; }
                bool is_luma() const { return mode() == color_mode::luma; }

                const rgba_matrix_t& rgba() const { return m_rgba; }
                const luma_matrix_t& luma() const { return m_luma; }

        private:

                ///
                /// \brief setup internal variables after a successfull loading
                ///
                bool setup_rgba();
                bool setup_luma();

        private:

                // attributes
                coord_t                 m_rows;
                coord_t                 m_cols;
                color_mode              m_mode;

                rgba_matrix_t           m_rgba;
                luma_matrix_t           m_luma;
        };

        typedef std::vector<image_t>    images_t;
}
