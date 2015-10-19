#pragma once

#include "min/problem.hpp"
#include <string>

namespace min
{
        namespace util
        {
                ///
                /// \brief compute the infinity-norm of a vector
                ///
                template
                <
                        typename tvector1
                >
                auto norm(const tvector1& a)
                {
                        return a.template lpNorm<Eigen::Infinity>();
                }

                ///
                /// \brief compute the infinity-distance between two vectors
                ///
                template
                <
                        typename tvector1,
                        typename tvector2
                >
                auto distance(const tvector1& a, const tvector2& b)
                {
                        return norm(a - b);
                }

                ///
                /// \brief map an array to an Eigen vector
                ///
                template
                <
                        typename tscalar,
                        typename tsize
                >
                auto map_vector(const tscalar* data, const tsize size)
                {
                        return Eigen::Map<const typename min::problem_t<tscalar>::tvector>(data, size);
                }

                template
                <
                        typename txmin,
                        typename txmins,
                        typename tscalar
                >
                bool check_close(const txmin& x, const txmins& xmins, const tscalar epsilon)
                {
                        for (const auto& xmin : xmins)
                        {
                                if (util::distance(x, util::map_vector(xmin.data(), x.size())) < epsilon)
                                {
                                        return true;
                                }
                        }

                        return false;
                }

                ///
                /// \brief numerical operations
                ///
                template 
                <
                        typename tscalar
                >
                tscalar square(const tscalar x)
                {
                        return x * x;
                }

                template 
                <
                        typename tscalar
                >
                tscalar cube(const tscalar x)
                {
                        return x * x * x;
                }

                template 
                <
                        typename tscalar
                >
                tscalar quartic(const tscalar x)
                {
                        return x * x * x * x;
                }
        }
}
