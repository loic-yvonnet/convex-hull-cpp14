/**
 * The Graham Scan algorithm requires sorting points
 * by polar angle. This algorithm is interesing in terms
 * of time and space complexity. This file provides
 * common angle computation facilities.
 * Some of these facilities are used only to check that
 * the algorihtms work as expected (in the unit tests).
 */

#ifndef angle_h
#define angle_h

#include "point_concept.hpp"
#include "point_math_utils.hpp"
#include "static_assert.hpp"

#include <cmath>

namespace hull {
    /**
     * Compute the angle made between vectors Oj and OP,
     * O being the origin point, P the input point, and
     * j the unit vector (1, 0).
     * It uses a slow trigonometric function. Hence, it 
     * should not be used in production.
     * @param p - the point P.
     * @return - the signed angle between Oj and OP in the
     *           range [-pi, pi].
     */
    template <typename TPoint>
    auto get_angle_with_j(const TPoint& p) {
        static_assert_is_point<TPoint>();
        return std::atan2(y(p), x(p));
    }
    
    /**
     * Same as above, but with a translation of the origin.
     * @param p - the point P.
     * @param origin - the origin 0.
     * @return - the signed angle between Oj and OP in the
     *           range [-pi, pi].
     */
    template <typename TPoint>
    auto get_angle_with_j(const TPoint& p, const TPoint& origin) {
        static_assert_is_point<TPoint>();
        return std::atan2(y(p) - y(origin), x(p) - x(origin));
    }
    
    /**
     * Compare the angle between OP1 and OP2. It computes the angles
     * to do so. Therefore, this function is slow and should not
     * be used in production code.
     * @param p1 - the point P1.
     * @param p2 - the point P2.
     * @return - true if angle(OP1) < angle(OP2).
     */
    template <typename TPoint>
    bool slow_compare_angles(const TPoint& p1, const TPoint& p2) {
        static_assert_is_point<TPoint>();
        const auto a1 = get_angle_with_j(p1);
        const auto a2 = get_angle_with_j(p2);
        return equals(a1, a2) ? square_norm(p1) < square_norm(p2) : a1 < a2;
    }
    
    /**
     * Same as above, but with a translation of the origin.
     * @param p1 - the point P1.
     * @param p2 - the point P2.
     * @param origin - the origin point O.
     * @return - true if angle(OP1) < angle(OP2).
     */
    template <typename TPoint>
    bool slow_compare_angles(const TPoint& p1, const TPoint& p2, const TPoint& origin) {
        static_assert_is_point<TPoint>();
        const auto a1 = get_angle_with_j(p1, origin);
        const auto a2 = get_angle_with_j(p2, origin);
        return equals(a1, a2) ? square_norm(p1 - origin) < square_norm(p2 - origin) : a1 < a2;
    }
    
    /**
     * Compare the angle between OP1 and OP2, without
     * actually computing the angles (no trigonometric
     * function involved).
     * Requirement: P1.y >= 0 && P2.y >= 0.
     * Based on a tip described in
     * https://stackoverflow.com/questions/16509100/sorting-points-by-their-polar-angle-in-java
     * @param p1 - the point P1.
     * @param p2 - the point P2.
     * @return - true if angle(OP1) < angle(OP2).
     */
    template <typename TPoint>
    constexpr bool compare_angles(const TPoint& p1, const TPoint& p2) {
        static_assert_is_point<TPoint>();
        
        using value_type = coordinate_t<TPoint>;
        value_type zero{};
        
        if (equals(y(p1), zero)) {
            if (equals(y(p2), zero)) {
                return x(p1) >= 0 && x(p2) < 0;
            }
            else {
                return x(p1) >= 0;
            }
        }
        else if (equals(y(p2), zero)) {
            return x(p2) < 0;
        }
        else {
            using div_type = std::conditional_t<std::is_integral<value_type>::value, double, value_type>;
            const auto x1 = static_cast<div_type>(x(p1));
            const auto y1 = static_cast<div_type>(y(p1));
            const auto x2 = static_cast<div_type>(x(p2));
            const auto y2 = static_cast<div_type>(y(p2));
            const auto div1 = -x1/y1;
            const auto div2 = -x2/y2;
            return equals(div1, div2) ? square_norm(p1) < square_norm(p2) : div1 < div2;
        }
    }
    
    /**
     * Same as above, but with a translation of the origin.
     * @param p1 - the point P1.
     * @param p2 - the point P2.
     * @param origin - the origin point O.
     * @return - true if angle(OP1) < angle(OP2).
     */
    template <typename TPoint>
    constexpr bool compare_angles(const TPoint& p1, const TPoint& p2, const TPoint& origin) {
        static_assert_is_point<TPoint>();
        return compare_angles(p1 - origin, p2 - origin);
    }
    
    /**
     * Compute the cross product between P1P2 and P1P3, as 
     * described in https://en.wikipedia.org/wiki/Graham_scan
     * @param p1 - the point P1.
     * @param p2 - the point P2.
     * @param p3 - the point P3.
     * @return -
     * <ul>
     *   <li>0: if P1, P2 and P3 are collinear.</li>
     *   <li>greater than 0: if P1, P2 and P3 are a counter-clockwise turn.</li>
     *   <li>lower than 0: if P1, P2 and P3 are clockwise turn.</li>
     * </ul>
     */
    template <typename TPoint>
    auto cross(const TPoint& p1, const TPoint& p2, const TPoint& p3)
    {
        static_assert_is_point<TPoint>();
        return (x(p2) - x(p1)) * (y(p3) - y(p1)) - (y(p2) - y(p1)) * (x(p3) - x(p1));
    }
}

#endif
