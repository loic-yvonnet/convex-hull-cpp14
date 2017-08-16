#ifndef angle_h
#define angle_h

#include "point_concept.hpp"

#include <cmath>
#include <limits>

namespace hull {
    template <typename T>
    constexpr bool equals(T a, T b) {
        constexpr const auto epsilon = std::numeric_limits<T>::epsilon();
        return (b - epsilon <= a) && (a <= b + epsilon);
    }
    
    template <typename TPoint>
    constexpr auto square_norm(const TPoint& p) {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        return x(p) * x(p) + y(p) * y(p);
    }
    
    template <typename TPoint>
    constexpr TPoint operator-(const TPoint& p1, const TPoint& p2) {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        const auto xp = x(p1) - x(p2);
        const auto yp = y(p1) - y(p2);
        return TPoint{xp, yp};
    }
    
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
                return x(p1) > 0;
            }
        }
        else if (equals(y(p2), zero)) {
            return x(p2) < 0;
        }
        else {
            const auto div1 = -x(p1)/y(p1);
            const auto div2 = -x(p2)/y(p2);
            return equals(div1, div2) ? square_norm(p1) < square_norm(p2) : div1 < div2;
        }
    }
}

#endif
