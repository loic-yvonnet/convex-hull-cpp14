/**
 * Implementation of Chan's algorithm for convex hull
 * computation of a set of points in the 2d space.
 */

#ifndef chan_algorithm_h
#define chan_algorithm_h

#include "angle.hpp"
#include "graham_scan.hpp"
#include "jarvis_march.hpp"
#include "point_concept.hpp"
#include "static_assert.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <experimental/optional>
#include <utility>

namespace hull::algorithms::details::chan {
    /**
     * Compute the number of elements (i.e. distance) and the number
     * of partitions for the Chan's algorithm.
     * @param first - forward iterator to the first element of the container of points.
     * @param last - forward iterator to the one-past last point of the container.
     * @return - a pair containing the number of elements and the number of partitions.
     */
    template <typename ForwardIt>
    auto compute_distance_and_number_of_partitions(ForwardIt first, ForwardIt last, std::size_t m) {
        // Let r = ceil(n/m)
        const auto n = std::distance(first, last);
        const auto r = static_cast<std::size_t>(std::ceil(static_cast<double>(n) / static_cast<double>(m)));
        return std::make_pair(n, r);
    }
    
    /**
     * Simplified access to a partition. The Chan's algorithm requires to partition
     * P into disjoint subsets P(1), P(2), ..., P(r), each of size at most m.
     * We make it a no-op by taking the points in their original order, and assume
     * the subsets at indices [0 ; m - 1], [m ; 2m - 1], etc.
     * @param first - iterator to the first element of the container of points.
     * @param last - iterator to the one-past last point of the container.
     * @param r - the number of partitions.
     * @param m - the guessed number of points on the convex hull.
     * @return - a lambda function that takes an argument i and returns the
     *           iterator to the first element of the partition i.
     */
    template <typename RandomIt>
    auto partition(RandomIt first, RandomIt last, std::size_t r, std::size_t m) {
        return [first, last, r, m](std::size_t i) {
            return (i < r) ? (first + (i * m)) : last;
        };
    }
    
    /**
     * For each partition P(0), P(1), ..., P(r -  1), call Graham Scan's
     * algorithm. P(r - 1) may have less elements than the other partitions.
     * @param P - the partitions of points.
     * @param last - iterator to the one-past last point of the container.
     * @param r - the number of partitions.
     * @return - a vector of iterators. Graham Scan returns an iterator to the
     *           one-past last point forming the convex hull. For each partition,
     *           we need to keep track of this resulting iterator.
     */
    template <typename RandomIt, typename Partition>
    auto compute_graham_scan_for_each_partition(Partition P, RandomIt last, std::size_t r) {
        // Extra memory required to store the size of the sub-convex hulls
        std::vector<RandomIt> lasts;
        lasts.reserve(r);
        
        // For i = 1 to r do:
        //     Compute Hull(P(i)) using Graham's scan and store the vertices in an ordered array.
        for (std::size_t i{}; i < r; i++) {
            const auto convex_hull_last = graham_scan(P(i), P(i + 1));
            lasts.push_back(convex_hull_last);
        }
        
        return lasts;
    }
    
    /**
     * Get the bottom most point from a container of points.
     * @param first - forward iterator to the first element of the container of points.
     * @param last - forward iterator to the one-past last point of the container.
     * @return - iterator to the bottom most point of the container.
     */
    template <typename ForwardIt>
    ForwardIt get_bottom_most(ForwardIt first, ForwardIt last) {
        return std::min_element(first, last, [](const auto& p1, const auto& p2) {
            return y(p1) < y(p2) || (y(p1) == y(p2) && x(p1) > x(p2));
        });
    }
    
    /**
     * Merge the partitions that were formerly sorted with Graham Scan.
     * To do so, we use a subset of the Jarvis March algorithm, as described
     * in Chan's algorithm.
     * @param first - iterator to the first element of the container of points.
     * @param last - iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @param point_on_hull - the bottom most point on the convex hull.
     * @param P - the partitions of points.
     * @param lasts - random access container of iterators to the lasts elements of the partitions.
     * @param r - the number of partitions.
     * @param m - the guessed number of points on the convex hull.
     * @return - an optional iterator to the last element forming the convex hull of the
     *           destination container of points. This is an optional iterator because Chan's
     *           algorithm does not converge on the solution for m < h. If the merge does not
     *           converge, the returned optional is false.
     */
    template <
        typename RandomIt,
        typename OutputIt,
        typename Partition,
        typename TPoint = typename std::iterator_traits<RandomIt>::value_type,
        typename Lasts = std::vector<RandomIt>
    >
    std::experimental::optional<OutputIt> merge_partitions_with_jarvis_march(RandomIt first,
                                                                             RandomIt last,
                                                                             OutputIt first2,
                                                                             TPoint point_on_hull,
                                                                             Partition P,
                                                                             const Lasts& lasts,
                                                                             std::size_t r,
                                                                             std::size_t m)
    {
        // For k = 1 to m do:
        //     For i = 1 to r do:
        //         Compute point q in P(i) that maximizes the angle p(k-1)  p(k)  q
        //     Let p(k+1) be the point q in q(1),q(2),...q(r) than maximizes the angle p(k-1)  p(k)  q
        //     If p(k+1) = p(1) then return {p(1), p(2), ... p(k)}.
        using point_type = typename std::iterator_traits<RandomIt>::value_type;
        std::vector<point_type> q(r);
        
        const auto pfirst = point_on_hull;
        
        for (std::size_t k{}; k < m; k++) {
            *first2++ = point_on_hull;
            
            for (std::size_t i{}; i < r; i++) {
                q[i] = details::jarvis::next_point_on_hull(P(i), lasts[i], point_on_hull);
            }
            
            point_on_hull = details::jarvis::next_point_on_hull(std::begin(q), std::end(q), point_on_hull);
            
            if (hull::equals(point_on_hull, pfirst)) {
                return first2;
            }
        }
        
        // Return "m was too small, try again"
        return {};
    }
}

namespace hull::algorithms::details {
    /**
     * Implementation of Chan's algorithm, which is theoretically
     * the most performant in operational research terms.
     * This algorithm modifies the order of the input points.
     * Average time complexity: O(N * log(H)) where N is the input number of
     * points and H is the number of points on the resulting convex hull.
     * Average space complexity: O(2 * N).
     * Reference: http://www.cs.wustl.edu/~pless/506/l3.html
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @param m - the guessed number of points on the convex hull.
     * @return - an optional iterator to the last element forming the convex hull of the
     *           destination container of points. This is an optional iterator because Chan's
     *           algorithm does not converge on the solution for m < H. If the merge does not
     *           converge, the returned optional is false.
     */
    template <typename RandomIt, typename OutputIt>
    std::experimental::optional<OutputIt> chan_impl(RandomIt first, RandomIt last, OutputIt first2, std::size_t m) {
        if (first == last) {
            return {};
        }
        
        const auto [n, r] = chan::compute_distance_and_number_of_partitions(first, last, m);
        auto P = chan::partition(first, last, r, m);
        auto lasts = chan::compute_graham_scan_for_each_partition(P, last, r);
        auto point_on_hull = *chan::get_bottom_most(first, last);
        
        return chan::merge_partitions_with_jarvis_march(first, last, first2, point_on_hull, P, lasts, r, m);
    }
}

namespace hull::algorithms {
    /**
     * Compute the convex hull of a container of points following
     * Chan's algorithm. This algorithm modifies the input points.
     * Average time complexity: O(N * log(H)) where N is the number of
     * input points and H is the number of points on the convex hull.
     * Average space complexity: O(3 * N).
     * Reference: http://www.cs.wustl.edu/~pless/506/l3.html
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           destination container of points.
     */
    template <typename RandomIt, typename OutputIt>
    OutputIt chan(RandomIt first, RandomIt last, OutputIt first2) {
        static_assert_is_random_access_iterator_to_point<RandomIt>();
        
        if (first == last) {
            return first2;
        }
        
        // For t = 1; 2; 3... do:
        //     Let m = min(2^(2^t),n)
        //     Invoke PartialHull(P, m), returning the result in L.
        //     If L != "try again" then return L.
        using point_type = typename std::iterator_traits<RandomIt>::value_type;
        const std::size_t n = std::distance(first, last);
        std::vector<point_type> intermediary(n);
        
        for (std::size_t t{1}; ; t++) {
            const std::size_t pow = 1 << (1 << t); // warning: may overflow
            const auto m = std::min(pow, n);
            const auto last_intermediary = details::chan_impl(first, last, std::begin(intermediary), m);
            if (last_intermediary) {
                return std::move(std::begin(intermediary), *last_intermediary, first2);
            }
            intermediary.clear();
        }
    }
}

namespace hull {
    /**
     * Compile-time enumeration to choose the
     * algorithm thanks to a policy approach.
     * @param chan_t - Chan's algorithm.
     */
    struct chan_t {};
    
    /**
     * Algorithms policies to choose an overload.
     * @param chan - Chan's algorithm.
     */
    namespace choice {
        static constexpr const chan_t chan{};
    }
    
    /**
     * Overload of iterator-based convex hull computation for Chan.
     * Average time complexity: O(N * log(H)) where N is the number of input
     * points and H is the number of points on the convex hull.
     * Average space complexity: O(3 * N).
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           destination container of points.
     */
    template <typename RandomIt, typename OutputIt>
    auto compute_convex_hull(chan_t policy, RandomIt first, RandomIt last, OutputIt first2) {
        return algorithms::chan(first, last, first2);
    }
    
    namespace convex {
        /**
         * Overload of container-based convex hull computation for Chan.
         * Average time complexity: O(N * log(H)) where N is the number of input
         * points and H is the number of points on the convex hull.
         * Average space complexity: O(4 * N).
         * @param c1 - the input container.
         * @param c2 - the destination container.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(chan_t policy, TContainer1 c1, TContainer2& c2) {
            hull::algorithms::chan(std::begin(c1), std::end(c1), std::back_inserter(c2));
        }
    }
}

#endif
