#ifndef point_concept_h
#define point_concept_h

#include <type_traits>

#include "reflection.hpp"

namespace hull {
    HAS_MEMBER(x); // has_member_x
    HAS_MEMBER(X); // has_member_X
    HAS_MEMBER(y); // has_member_y
    HAS_MEMBER(Y); // has_member_Y
    
    template <typename TPoint>
    constexpr bool has_x_y() {
        return has_member_x<TPoint>::value && has_member_y<TPoint>::value;
    }
    
    template <typename TPoint>
    constexpr bool has_X_Y() {
        return has_member_X<TPoint>::value && has_member_Y<TPoint>::value;
    }
    
    template <typename TPoint, typename TEnable = void>
    struct is_point: std::false_type {};
    
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<has_x_y<TPoint>()>>: std::true_type {};
    
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<has_X_Y<TPoint>()>>: std::true_type {};
    
    template <typename TPoint>
    constexpr bool is_point_v() {
        return is_point<TPoint>::value;
    }
    
    template <typename TPoint>
    auto x(const TPoint& p) -> decltype(p.x) {
        return p.x;
    }
    
    template <typename TPoint>
    auto x(const TPoint& p) -> decltype(p.X) {
        return p.X;
    }
    
    template <typename TPoint>
    auto y(const TPoint& p) -> decltype(p.y) {
        return p.y;
    }
    
    template <typename TPoint>
    auto y(const TPoint& p) -> decltype(p.Y) {
        return p.Y;
    }
}

#endif
