/**
 * Simple bounding box algorithm.
 * This is not really a convex hull algorithm.
 */

#ifndef bounding_box_h
#define bounding_box_h

#include "point_concept.hpp"

#include <algorithm>

namespace hull::algorithms {
    /**
     * AABB bounding box implementation.
     * This is not a convex hull algorithm but like convex hull, it
     * surrounds the input set of points.
     * The bounding box points are returned counter-clockwise from the
     * bottom-left side.
     * Average time complexity: O(N) where N is the number of points.
     * Average space complexity: O(N).
     * @param first - the forward iterator to the first point of the container.
     * @param last - the forward iterator to the one-past last point of the container.
     * @param first2 - the forward iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the bounding box of the
     *           provided container of points.
     */
    template <typename ForwardIt1, typename ForwardIt2>
    ForwardIt2 bounding_box(ForwardIt1 first, ForwardIt1 last, ForwardIt2 first2) {
        using point_type = typename std::iterator_traits<ForwardIt1>::value_type;
        
        static_assert(is_point_v<point_type>(), "ill-formed point");
        static_assert(std::is_base_of<
                          std::forward_iterator_tag,
                          typename std::iterator_traits<ForwardIt1>::iterator_category
                      >(), "forward access iterator required");
        
        auto less_x = [](const auto& p1, const auto& p2) {
            return x(p1) < x(p2);
        };
        auto less_y = [](const auto& p1, const auto& p2) {
            return y(p1) < y(p2);
        };
        
        const auto [min_x, max_x] = std::minmax_element(first, last, less_x);
        const auto [min_y, max_y] = std::minmax_element(first, last, less_y);
        
        *first2++ = point_type{x(*min_x), y(*min_y)};
        *first2++ = point_type{x(*max_x), y(*min_y)};
        *first2++ = point_type{x(*max_x), y(*max_y)};
        *first2   = point_type{x(*min_x), y(*max_y)};
        
        return first2;
    }
}

#endif
