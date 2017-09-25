/**
 * Some common template functions math utilities
 * that do not belong anywhere else.
 */

#ifndef math_utils_h
#define math_utils_h

#include "static_assert.hpp"

#include <limits>
#include <type_traits>

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
}

#endif
