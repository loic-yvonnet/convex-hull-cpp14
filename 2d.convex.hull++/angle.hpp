/**
 * The Graham Scan algorithm requires sorting points
 * by polar angle. This algorithm is interesing in terms
 * of time and space complexity. This file provides
 * common angle computation facilities
 */

#ifndef angle_h
#define angle_h

#include "point_concept.hpp"
#include "utils.hpp"

#include <cmath>
#include <limits>

namespace hull {
    template <typename TPoint>
    auto get_angle_with_j(const TPoint& p) {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        return std::atan2(y(p), x(p));
    }
    
    template <typename TPoint>
    auto get_angle_with_j(const TPoint& p, const TPoint& origin) {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        return std::atan2(y(p) - y(origin), x(p) - x(origin));
    }
    
    template <typename TPoint>
    bool slow_compare_angles(const TPoint& p1, const TPoint& p2) {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        const auto a1 = get_angle_with_j(p1);
        const auto a2 = get_angle_with_j(p2);
        return equals(a1, a2) ? square_norm(p1) < square_norm(p2) : a1 < a2;
    }
    
    template <typename TPoint>
    bool slow_compare_angles(const TPoint& p1, const TPoint& p2, const TPoint& origin) {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        const auto a1 = get_angle_with_j(p1, origin);
        const auto a2 = get_angle_with_j(p2, origin);
        return equals(a1, a2) ? square_norm(p1 - origin) < square_norm(p2 - origin) : a1 < a2;
    }
    
    // Based on a tip described in
    // https://stackoverflow.com/questions/16509100/sorting-points-by-their-polar-angle-in-java
    // Works only in the upper quadrants
    template <typename TPoint>
    constexpr bool compare_angles(const TPoint& p1, const TPoint& p2) {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        
        using value_type = decltype(x(p1));
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
    
    template <typename TPoint>
    constexpr bool compare_angles(const TPoint& p1, const TPoint& p2, const TPoint& origin) {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        return compare_angles(p1 - origin, p2 - origin);
    }
    
    // As per https://en.wikipedia.org/wiki/Graham_scan
    template <typename TPoint>
    auto cross(const TPoint& p1, const TPoint& p2, const TPoint& p3)
    {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        return (x(p2) - x(p1)) * (y(p3) - y(p1)) - (y(p2) - y(p1)) * (x(p3) - x(p1));
    }
}

#endif
