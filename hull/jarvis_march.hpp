/**
 * Implementation of Jarvis March for convex hull
 * computation of a set of points in the 2d space.
 */

#ifndef jarvis_march_h
#define jarvis_march_h

#include "angle.hpp"
#include "point_concept.hpp"
#include "static_assert.hpp"

#include <algorithm>

namespace hull::algorithms::details::jarvis {
    /**
     * Create a lambda which tells whether a given point
     * sj is on the left of the vector p0p1.
     * @param p0 - the first point of the vector.
     * @param p1 - the second point of the vector.
     * @return - a lambda function which takes a point sj
     *           and returns true if this point is on the
     *           left of the vector p0p1.
     */
    template <typename TPoint>
    auto is_on_the_left(const TPoint& p0, const TPoint& p1) {
        return [&p0, &p1](const auto& sj) {
            const auto res = cross(p0, p1, sj);
            
            // If they are collinear, we take the farthest point from pi
            if (hull::equals(res, static_cast<decltype(res)>(0))) {
                return hull::square_norm(sj - p0) > hull::square_norm(p1 - p0);
            }
            
            return res > 0;
        };
    }
}

namespace hull::algorithms::details {
    /**
     *
     */
    template <
        typename RandomIt,
        typename TPoint = typename RandomIt::value_type
    >
    TPoint max_jarvis_march(RandomIt first, RandomIt last, const TPoint& point_on_hull) {
        auto endpoint = point_on_hull;
        
        // Helper lambda to determine wheter sj is on the left of line pi to endpoint
        auto is_on_the_left = jarvis::is_on_the_left(point_on_hull, endpoint);
        
        std::for_each(first, last, [&endpoint, &point_on_hull, is_on_the_left](const auto& sj) {
            // if (endpoint == pointOnHull) or (S[j] is on left of line from P[i] to endpoint)
            if (hull::equals(endpoint, point_on_hull) || is_on_the_left(sj)) {
                // found greater left turn, update endpoint
                endpoint = sj;
            }
        });
        
        return endpoint;
    }
}

namespace hull::algorithms {
    /**
     * Compute the convex hull of a container of points following
     * the Jarvis March algorithm.
     * Average time complexity: O(N * H) where N is the number of input
     * points and H the number of points on the convex hull.
     * Average space complexity: O(N + H).
     * Reference: https://en.wikipedia.org/wiki/Gift_wrapping_algorithm
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           provided container of points.
     */
    template <typename RandomIt1, typename RandomIt2>
    RandomIt2 jarvis_march(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        static_assert_is_random_access_iterator_to_point<RandomIt1>();
        static_assert_is_random_access_iterator_to_point<RandomIt2>();
        
        const auto N = std::distance(first, last);
        if (N <= 1) {
            return std::copy(first, last, first2);
        }
        
        // leftmost point
        auto point_on_hull = *std::min_element(first, last, [](const auto& p1, const auto& p2) {
            return x(p1) < x(p2) || (x(p1) == x(p2) && y(p1) > y(p2));
        });
        
        // Repeat until wrapped around to first hull point
        std::size_t i{};
        do {
            // P[i] = pointOnHull
            *(first2 + i) = point_on_hull;
            
            point_on_hull = details::max_jarvis_march(first, last, point_on_hull);
            
            i++;
        }
        while (!hull::equals(point_on_hull, *first2));
        
        return first2 + i;
    }
}

namespace hull {
    /**
     * Compile-time enumeration to choose the
     * algorithm thanks to a policy approach.
     * @param jarvis_march_t - Jarvis March algorithm.
     */
    struct jarvis_march_t {};
    
    /**
     * Algorithms policies to choose an overload.
     * @param jarvis_march - Jarvis March algorithm.
     */
    namespace choice {
        static constexpr const jarvis_march_t jarvis_march{};
    }
    
    /**
     * Overload of iterator-based convex hull computation for Jarvis March.
     * Average time complexity: O(N * H) where N is the number of input
     * points and H is the number of points on the convex hull.
     * Average space complexity: O(2 * N).
     * It is unfortunately not possible to use a std::back_insert_iterator.
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           destination container of points.
     */
    template <typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(jarvis_march_t policy, RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        using value_type = typename std::iterator_traits<RandomIt2>::value_type;
        std::fill_n(first2, std::distance(first, last), value_type{});
        
        return algorithms::jarvis_march(first, last, first2);
    }
    
    namespace convex {
        /**
         * Overload of container-based convex hull computation for Jarvis March.
         * Average time complexity: O(N * H) where N is the number of input
         * points and H is the number of points on the convex hull.
         * Average space complexity: O(2 * N).
         * @param c1 - the input container.
         * @param c2 - the destination container.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(jarvis_march_t policy, const TContainer1& c1, TContainer2& c2) {
            c2.resize(c1.size());
            
            std::fill(std::begin(c2), std::end(c2), typename TContainer2::value_type{});
            auto last = hull::algorithms::jarvis_march(std::begin(c1), std::end(c1), std::begin(c2));
            
            c2.erase(last, std::end(c2));
        }
    }
}

#endif
