/**
 * Implementation of Chan's algorithm for convex hull
 * computation of a set of points in the 2d space.
 */

#ifndef chan_algorithm_h
#define chan_algorithm_h

#include "angle.hpp"
#include "assert.hpp"
#include "graham_scan.hpp"
#include "jarvis_march.hpp"
#include "point_concept.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <experimental/optional>
#include <utility>

namespace hull::algorithms::details {
    /**
     * Implementation of Chan algorithm, which is theoretically
     * the most performant in operational research terms.
     * Reference: http://www.cs.wustl.edu/~pless/506/l3.html
     */
    template <typename RandomIt, typename OutputIt>
    std::experimental::optional<OutputIt> chan_impl(RandomIt first, RandomIt last, OutputIt first2, std::size_t m) {
        if (first == last) {
            return {};
        }
        
        // (1) Let r = ceil(n/m).
        const auto n = std::distance(first, last);
        const auto r = static_cast<std::size_t>(std::ceil(static_cast<double>(n) / static_cast<double>(m)));
        
        // Partition P into disjoint subsets P(1),P(2),... P(r), each of size at most m.
        // no-op: we take the points in their original order, and assume the subsets at
        // indices [0 ; m - 1], [m ; 2m - 1], etc.
        // We define a lambda to clarify the access to P(i):
        auto P = [first, m](auto i) {
            return first + (i * m);
        };
        
        // Extra memory required to store the size of the sub-convex hulls
        std::vector<RandomIt> lasts;
        lasts.reserve(r);
        
        // (2) For i = 1 to r do:
        //     (a) Compute Hull(P(i)) using Graham's scan and store the vertices in an ordered array.
        for (std::size_t i{}; i < r - 1; i++) {
            const auto convex_hull_last = graham_scan(P(i), P(i + 1));
            lasts.push_back(convex_hull_last);
        }
        
        // Last subset, which may be smaller
        {
            const auto convex_hull_last = graham_scan(P(r - 1), last);
            lasts.push_back(convex_hull_last);
        }
        
        // (3) Let point_on_hull be the bottommost point of P
        const auto min_y = std::min_element(first, last, [](const auto& p1, const auto& p2) {
            return y(p1) < y(p2) || (y(p1) == y(p2) && x(p1) > x(p2));
        });
        auto point_on_hull = *min_y;
        const auto pfirst = *min_y;
        
        // Let endpoint = (-Inf; 0)
        using point_type = decltype(point_on_hull);
        using coord_type = std::remove_reference_t<decltype(x(std::declval<point_type>()))>;
        auto endpoint = point_type{std::numeric_limits<coord_type>::lowest(), static_cast<coord_type>(0)};
        
        // (4) For k = 1 to m do:
        //     (a) For i = 1 to r do:
        //         Compute point q in P(i) that maximizes the angle p(k-1)  p(k)  q
        //     (b) Let p(k+1) be the point q in q(1),q(2),...q(r) than maximizes the angle p(k-1)  p(k)  q
        //     (c) If p(k+1) = p(1) then return {p(1), p(2), ... p(k)}.
        std::vector<point_type> q(r);
        for (std::size_t k{}; k < m; k++) {
            *first2++ = point_on_hull;
            
            for (std::size_t i{}; i < r; i++) {
                q[i] = max_jarvis_march(P(i), lasts[i], point_on_hull/*, endpoint*/);
            }
            
            const auto pk = details::max_jarvis_march(std::begin(q), std::end(q), point_on_hull/*, endpoint*/);
            
            if (hull::equals(pk, pfirst)) {
                return first2;
            }
            
            endpoint = point_on_hull;
            point_on_hull = pk;
        }
        
        // (5) Return "m was too small, try again"
        return {};
    }
}

namespace hull::algorithms {
    /**
     *
     */
    template <typename RandomIt, typename OutputIt>
    OutputIt chan(RandomIt first, RandomIt last, OutputIt first2) {
        static_assert_is_random_access_iterator_to_point<RandomIt>();
        
        if (first == last) {
            return first2;
        }
        
        // (1) For t = 1; 2; 3... do:
        //     (a) Let m = min(2^(2^t),n)
        //     (b) Invoke PartialHull(P, m), returning the result in L.
        //     (c) If L != "try again" then return L.
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
