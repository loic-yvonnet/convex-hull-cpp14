#ifndef utils_h
#define utils_h

namespace hull {
    template <
        typename T,
        typename = std::enable_if_t<std::is_floating_point<T>::value>
    >
    constexpr bool equals(T a, T b) {
        constexpr const auto epsilon = std::numeric_limits<T>::epsilon();
        return (b - epsilon <= a) && (a <= b + epsilon);
    }
    
    template <
        typename T,
        typename std::enable_if_t<std::is_integral<T>::value, int> = 0
    >
    constexpr bool equals(T a, T b) {
        return a == b;
    }
    
    template <
        typename TPoint,
        typename std::enable_if_t<is_point_v<TPoint>(), int> = 0
    >
    constexpr bool equals(const TPoint& p1, const TPoint& p2) {
        return equals(x(p1), x(p2)) && equals(y(p1), y(p2));
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
}

#endif
