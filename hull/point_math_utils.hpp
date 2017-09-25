/**
 * Some common template functions math utilities
 * that do not belong anywhere else.
 */

#ifndef point_math_utils_h
#define point_math_utils_h

#include "point_concept.hpp"
#include "math_utils.hpp"
#include "static_assert.hpp"

#include <type_traits>

namespace hull {
    /**
     * Compare 2 points, relying on the above functions to
     * compare their internal coordinates.
     * @param p1 - the 1st point.
     * @param p2 - the 2nd point.
     * @return - true if p1 and p2 are equal.
     */
    template <
        typename TPoint,
        typename std::enable_if_t<is_point_v<TPoint>(), int> = 0
    >
    constexpr bool equals(const TPoint& p1, const TPoint& p2) {
        return equals(x(p1), x(p2)) && equals(y(p1), y(p2));
    }
    
    /**
     * Compute the square norm of the vector made of
     * the input point P with the origin O: |OP|2.
     * @param p - the point P.
     * @return - the square of the norm.
     */
    template <typename TPoint>
    constexpr auto square_norm(const TPoint& p) {
        static_assert_is_point<TPoint>();
        return x(p) * x(p) + y(p) * y(p);
    }
    
    /**
     * Substract 2 points.
     * @param p1 - the 1st point.
     * @param p2 - the 2nd point.
     * @return - a new point whose coordinates are p1 - p2.
     */
    template <typename TPoint>
    constexpr TPoint operator-(const TPoint& p1, const TPoint& p2) {
        static_assert_is_point<TPoint>();
        const auto xp = x(p1) - x(p2);
        const auto yp = y(p1) - y(p2);
        return make_point<TPoint>(xp, yp);
    }
}

#endif
