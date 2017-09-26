/**
 * Implementation of Monotone Chain for convex hull
 * computation of a set of points in the 2d space.
 */

#ifndef monotone_chain_h
#define monotone_chain_h

#include "angle.hpp"
#include "point_concept.hpp"
#include "static_assert.hpp"

#include <algorithm>
#include <tuple>

namespace hull::algorithms::details::monotone {
    /**
     * Sort the points of P by x-coordinate (in case of a tie, sort by y-coordinate).
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     */
    template <typename RandomIt>
    void sort(RandomIt first, RandomIt last) {
        std::sort(first, last, [](const auto& p1, const auto& p2) {
            return (x(p1) < x(p2) || (hull::equals(x(p1), x(p2)) && y(p1) < y(p2)));
        });
    }
    
    /**
     * Build a lambda function which tells whether a given point
     * (the one at index i with respect to first iterator) is on the
     * left (that is, not counter-clockwise) of the vector made of
     * the last 2 points on the result convex hull.
     * @param k - the index of the last point on the convex hull.
     * @param first - iterator to the first point in the input container of points.
     * @param first2 - iterator to the first point in the resulting convex hull.
     * @return - a lambda function which takes an index i and tells whether
     *           the point at first+i is on the left of vector first2+(k-2) first2+(k-1).
     */
    template <typename RandomIt1, typename RandomIt2>
    auto no_counter_clockwise(std::size_t& k, RandomIt1 first, RandomIt2 first2) {
        return [&k, first, first2](auto i) {
            return cross(*(first2 + (k - 2)), *(first2 + (k - 1)), *(first + i)) <= 0;
        };
    };
    
    /**
     * Build a lambda function which copies the input point at
     * index i into the resulting convex hull at index k (and it
     * increment k, the number of points in the convex hull).
     * @param k - the number of points in the resulting convex hull.
     * @param first - iterator to the first point in the input container of points.
     * @param first2 - iterator to the first point in the resulting convex hull.
     * @return - a lambda function which copies first+i to first2+k.
     */
    template <typename RandomIt1, typename RandomIt2>
    auto copy(std::size_t& k, RandomIt1 first, RandomIt2 first2) {
        return [&k, first, first2](auto i) {
            *(first2 + k) = *(first + i);
            k++;
        };
    }
    
    /**
     * Get all the dependencies of the lower_hull and upper_hull algorithms.
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @param k - this will be the number of points on the convex hull.
     * @return - a tuple containing all the dependencies of the lower and upper hull algorithms.
     */
    template <typename RandomIt1, typename RandomIt2>
    auto get_dependencies(RandomIt1 first, RandomIt1 last, RandomIt2 first2, std::size_t& k) {
        return std::make_tuple(no_counter_clockwise(k, first, first2),
                               copy(k, first, first2),
                               std::distance(first, last));
    }
    
    /**
     * Compute the lower hull of the convex hull.
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @param k - this will be the number of points on the lower hull.
     */
    template <typename RandomIt1, typename RandomIt2>
    void lower_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2, std::size_t& k) {
        auto [no_counter_clockwise, copy, N] = get_dependencies(first, last, first2, k);
        
        for (int i{}; i < N; i++) {
            while (k >= 2 && no_counter_clockwise(i)) {
                k--;
            }
            copy(i);
        }
    }

    /**
     * Compute the upper hull of the convex hull.
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @param k - this will be the number of points on the upper hull.
     */
    template <typename RandomIt1, typename RandomIt2>
    void upper_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2, std::size_t& k) {
        auto [no_counter_clockwise, copy, N] = get_dependencies(first, last, first2, k);
        
        auto t = k + 1;
        for (int i{static_cast<int>(N - 2)}; i >= 0; i--) {
            while (k >= t && no_counter_clockwise(i)) {
                k--;
            }
            copy(i);
        }
    }
}

namespace hull::algorithms::details {
    /**
     * Compute the convex hull of a container of points following
     * the Monotone Chain algorithm. This algorithm does not work in-place, but
     * it still modifies the input points (it sorts them by x-coordinates).
     * Average time complexity: O(N * log(N)) where N is the number of
     * points.
     * Average space complexity: O(3 * N).
     * Reference: https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           provided container of points.
     */
    template <typename RandomIt1, typename RandomIt2>
    RandomIt2 monotone_chain_impl(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        monotone::sort(first, last);
        
        std::size_t k{};
        monotone::lower_hull(first, last, first2, k);
        monotone::upper_hull(first, last, first2, k);
        
        return first2 + (k - 1);
    }
}

namespace hull::algorithms {
    /**
     * Compute the convex hull of a container of points following
     * the Monotone Chain algorithm. This algorithm does not work in-place, but
     * it still modifies the input points (it sorts them by x-coordinates).
     * Average time complexity: O(N * log(N)) where N is the number of
     * points.
     * Average space complexity: O(3 * N).
     * Reference: https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           provided container of points.
     */
    template <typename RandomIt1, typename RandomIt2>
    RandomIt2 monotone_chain(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        static_assert_is_random_access_iterator_to_point<RandomIt1>();
        static_assert_is_random_access_iterator_to_point<RandomIt2>();
        
        const auto N = std::distance(first, last);
        if (N <= 1) {
            return std::copy(first, last, first2);
        }
        
        return details::monotone_chain_impl(first, last, first2);
    }
}

namespace hull {
    /**
     * Compile-time enumeration to choose the
     * algorithm thanks to a policy approach.
     * @param monotone_chain_t - Monotone Chain algorithm.
     */
    struct monotone_chain_t {};
    
    /**
     * Algorithms policies to choose an overload.
     * @param monotone_chain - Monotone Chain algorithm.
     */
    namespace choice {
        static constexpr const monotone_chain_t monotone_chain{};
    }
    
    /**
     * Overload of iterator-based convex hull computation for Monotone Chain.
     * Note that the input is modified.
     * Average time complexity: O(N * log(N)) where N is the number of
     * points.
     * Average space complexity: O(3 * N).
     * It is unfortunately not possible to use a std::back_insert_iterator.
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           destination container of points.
     */
    template <typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(monotone_chain_t policy, RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        using value_type = typename std::iterator_traits<RandomIt2>::value_type;
        std::fill_n(first2, 2 * std::distance(first, last), value_type{});
        
        return algorithms::monotone_chain(first, last, first2);
    }
    
    namespace convex {
        /**
         * Overload of container-based convex hull computation for Monotone Chain.
         * Average time complexity: O(N * log(N)) where N is the number of
         * points.
         * Average space complexity: O(4 * N).
         * @param c1 - the input container.
         * @param c2 - the destination container.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(monotone_chain_t policy, TContainer1 c1, TContainer2& c2) {
            c2.resize(2 * c1.size());
            
            std::fill(std::begin(c2), std::end(c2), typename TContainer2::value_type{});
            auto last = hull::algorithms::monotone_chain(std::begin(c1), std::end(c1), std::begin(c2));
            
            c2.erase(last, std::end(c2));
        }
    }
}

#endif
