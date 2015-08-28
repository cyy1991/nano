#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "test_image"

#include <boost/test/unit_test.hpp>
#include "nanocv/vision/image.h"
#include "nanocv/math/random.hpp"

BOOST_AUTO_TEST_CASE(test_image_construct)
{
        using namespace ncv;

        for (size_t test = 0; test < 16; test ++)
        {
                random_t<coord_t> rng(16, 64);

                const auto rows = rng();
                const auto cols = rng();
                const auto mode = (test % 2 == 0) ? color_mode::luma : color_mode::rgba;

                image_t image(rows, cols, mode);

                BOOST_CHECK_EQUAL(image.is_luma(), mode == color_mode::luma);
                BOOST_CHECK_EQUAL(image.is_rgba(), mode == color_mode::rgba);
                BOOST_CHECK_EQUAL(image.rows(), rows);
                BOOST_CHECK_EQUAL(image.cols(), cols);
                BOOST_CHECK_EQUAL(image.size(), rows * cols);
                BOOST_CHECK(image.mode() == mode);

                BOOST_CHECK_EQUAL(image.luma().rows(), image.is_luma() ? rows : 0);
                BOOST_CHECK_EQUAL(image.luma().cols(), image.is_luma() ? cols : 0);

                BOOST_CHECK_EQUAL(image.rgba().rows(), image.is_rgba() ? rows : 0);
                BOOST_CHECK_EQUAL(image.rgba().cols(), image.is_rgba() ? cols : 0);

                image.make_luma();
                BOOST_CHECK_EQUAL(image.is_luma(), true);
                BOOST_CHECK(image.mode() == color_mode::luma);

                image.make_rgba();
                BOOST_CHECK_EQUAL(image.is_rgba(), true);
                BOOST_CHECK(image.mode() == color_mode::rgba);
        }
}

BOOST_AUTO_TEST_CASE(test_image_io_matrix)
{
        using namespace ncv;

        for (size_t test = 0; test < 16; test ++)
        {
                random_t<coord_t> rng(16, 64);

                const auto rows = rng();
                const auto cols = rng();

                if (test % 2 == 0)
                {
                        const auto rgba = color::make_random_rgba();

                        rgba_matrix_t data(rows, cols);
                        data.setConstant(rgba);

                        image_t image;
                        BOOST_CHECK_EQUAL(image.load_luma(data), true);
                        BOOST_CHECK_EQUAL(image.is_luma(), true);
                        BOOST_CHECK_EQUAL(image.is_rgba(), false);
                        BOOST_CHECK_EQUAL(image.rows(), rows);
                        BOOST_CHECK_EQUAL(image.cols(), cols);
                        BOOST_REQUIRE(data.size() == image.luma().size());

//                        const auto& luma = image.luma();
//                        for (int i = 0; i < luma.size(); i ++)
//                        {
//                                BOOST_CHECK_EQUAL(luma(i), color::make_luma(data(i)));
//                        }
                }
        }
}


BOOST_AUTO_TEST_CASE(test_image_io)
{
        using namespace ncv;

        for (size_t test = 0; test < 16; test ++)
        {
                random_t<coord_t> rng(16, 64);

                const auto rows = rng();
                const auto cols = rng();
                const auto mode = (test % 2 == 0) ? color_mode::luma : color_mode::rgba;

                image_t image(rows, cols, mode);

                BOOST_CHECK_EQUAL(image.is_luma(), mode == color_mode::luma);
                BOOST_CHECK_EQUAL(image.is_rgba(), mode == color_mode::rgba);
                BOOST_CHECK_EQUAL(image.rows(), rows);
                BOOST_CHECK_EQUAL(image.cols(), cols);
        }

//        * create images (RGBA + grayscale) - check basic accessors
//        * save images to disk (png) - check save
//        * load images from disk - check loading and that same pixels are loaded
//        * transform to graycale/rgba - check results
//        * load image from disk as opposite type
//        * check saving to scaled tensors
}

BOOST_AUTO_TEST_CASE(test_image_tensor)
{
//        * create images (RGBA + grayscale) - check basic accessors
//        * save images to disk (png) - check save
//        * load images from disk - check loading and that same pixels are loaded
//        * transform to graycale/rgba - check results
//        * load image from disk as opposite type
//        * check saving to scaled tensors
}
