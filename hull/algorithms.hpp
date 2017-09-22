/**
 * This file contains the convex hull computation algorithms.
 * More about convex hull: https://en.wikipedia.org/wiki/Convex_hull
 * You may refer to the unit tests to see how to use this
 * small library.
 * The provided algorithms are mostly iterator-based, following the
 * STL idioms for generality. However, container-based versions are
 * provided for ease-of-use.
 * Example:
 *      <code>
 *      struct point { double x; double y; }
 *      std::vector<point> points = {{5., 2.}, ... };
 *      std::vector<point> convex_hull;
 *      hull::convex::compute(points, convex_hull);
 *      </code>
 */

#ifndef algorithms_h
#define algorithms_h

#include "bounding_box.hpp"
#include "chan_algorithm.hpp"
#include "graham_scan.hpp"
#include "jarvis_march.hpp"
#include "monotone_chain.hpp"

namespace hull {
    /**
     * Policy-based approach to call one of the previous functions.
     */
    template <typename Policy, typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        return compute_convex_hull(Policy{}, first, last, first2);
    }
    
    /**
     * The default policy is Graham Scan for the moment. A benchmark
     * is required between Graham Scan and Chan's algorithm. In theory,
     * Chan's algorithm is better. However, the practical implementation
     * of Chan's algorithm requires allocations and more space. Therefore,
     * on current computer architectures, Graham Scan is likely faster.
     */
    template <typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        return compute_convex_hull(choice::graham_scan, first, last, first2);
    }
    
    namespace convex {
        /**
         * Policy-based approach to call one of the previous functions.
         */
        template <typename Policy, typename TContainer1, typename TContainer2>
        void compute(const TContainer1& c1, TContainer2& c2) {
            compute(Policy{}, c1, c2);
        }
        
        /**
         * The default policy is Graham Scan.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(const TContainer1& c1, TContainer2& c2) {
            compute<hull::graham_scan_t>(c1, c2);
        }
    }
}

#endif
