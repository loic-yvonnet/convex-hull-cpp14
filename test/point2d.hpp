/**
 * Unit tests for the convex hull algorithms.
 */

#ifndef point2d_h
#define point2d_h

/**
 * Simple point structure used to test
 * the convex hull algorithms.
 */
struct point2d {
    int x{};
    int y{};
};

/**
 * Equality operator for std::equal.
 */
inline bool operator==(const point2d& p1, const point2d& p2) {
    return hull::equals(p1, p2);
}

#endif
