/**
 * Some common template functions utilities that do
 * do belong anywhere else.
 */

#ifndef utils_h
#define utils_h

namespace hull {
    /**
     * Floating-point computations involve numerical errors
     * that must be taken into account.
     * This function compares 2 floating point numbers with
     * an epsilon to avoid most numerical errors.
     * @param a - the 1st floating-point value.
     * @param b - the 2nd floating-point value.
     * @return - true if a ~= b.
     */
    template <
        typename T,
        typename = std::enable_if_t<std::is_floating_point<T>::value>
    >
    constexpr bool equals(T a, T b) {
        constexpr const auto epsilon = std::numeric_limits<T>::epsilon();
        return (b - epsilon <= a) && (a <= b + epsilon);
    }

    /**
     * Compare 2 integral values.
     * Integral computations are safer.
     * @param a - the 1st integral value.
     * @param b - the 2nd integral value.
     * @return - true if a == b.
     */
    template <
        typename T,
        typename std::enable_if_t<std::is_integral<T>::value, int> = 0
    >
    constexpr bool equals(T a, T b) {
        return a == b;
    }

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
        static_assert(is_point_v<TPoint>(), "ill-formed point");
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
        static_assert(is_point_v<TPoint>(), "ill-formed point");
        const auto xp = x(p1) - x(p2);
        const auto yp = y(p1) - y(p2);
        return TPoint{xp, yp};
    }
}

#endif
