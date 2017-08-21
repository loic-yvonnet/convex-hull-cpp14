/**
 * Point Concept (C++14)
 * Defining a point structure/class is very easy, and most libraries and
 * frameworks provide their own point. Therefore, a library must not
 * introduce its own point, but rather it should enable existing points to
 * be used naturally.
 * The C++ concept keyword does not exist in C++14/C++17. Thus, this small
 * set of facilities rely on SFINAE to support classic definitions of points.
 * It provides type traits to determine whether a type T supports the concept
 * of point, and a generic way to access its X and Y coordinates.
 * It is possible to support other kinds of points by providing a template
 * specialization for is_point, x() and y().
 */

#ifndef point_concept_h
#define point_concept_h

#include <type_traits>

#include "reflection.hpp"

namespace hull {
    /**
     * Generation of type traits to introspect client point
     * definition.
     */
    HAS_MEMBER(x); // has_member_x
    HAS_MEMBER(X); // has_member_X
    HAS_MEMBER(y); // has_member_y
    HAS_MEMBER(Y); // has_member_Y
    HAS_METHOD(x); // has_method_x
    HAS_METHOD(X); // has_method_X
    HAS_METHOD(y); // has_method_y
    HAS_METHOD(Y); // has_method_Y
    
    /**
     * Tells whether a given TPoint has x and y data members.
     */
    template <typename TPoint>
    constexpr bool has_x_y() {
        return has_member_x<TPoint>::value && has_member_y<TPoint>::value;
    }
    
    /**
     * Tells whether a given TPoint has X and Y data members.
     */
    template <typename TPoint>
    constexpr bool has_X_Y() {
        return has_member_X<TPoint>::value && has_member_Y<TPoint>::value;
    }
    
    /**
     * Tells whether a given TPoint has x and y methods.
     */
    template <typename TPoint>
    constexpr bool has_x_y_methods() {
        return has_method_x<TPoint>::value && has_method_y<TPoint>::value;
    }
    
    /**
     * Tells whether a given TPoint has X and Y methods.
     */
    template <typename TPoint>
    constexpr bool has_X_Y_methods() {
        return has_method_X<TPoint>::value && has_method_Y<TPoint>::value;
    }
    
    /**
     * By default, a type TPoint is not a point, unless there is
     * a template specialization that says otherwise.
     */
    template <typename TPoint, typename TEnable = void>
    struct is_point: std::false_type {};
    
    /**
     * If the type TPoint has x and y data members, then it fits
     * the point concept requirements.
     */
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<has_x_y<TPoint>()>>: std::true_type {};
    
    /**
     * If the type TPoint has X and Y data members, then it fits
     * the point concept requirements.
     */
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<has_X_Y<TPoint>()>>: std::true_type {};
    
    /**
     * If the type TPoint has x() and y() methods, then it fits
     * the point concept requirements.
     */
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<has_x_y_methods<TPoint>()>>: std::true_type {};
    
    /**
     * If the type TPoint has X() and Y() methods, then it fits
     * the point concept requirements.
     */
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<has_X_Y_methods<TPoint>()>>: std::true_type {};
    
    /**
     * If the type TPoint is an array with at least 2 elements, then it fits
     * the point concept requirements.
     */
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<std::is_array<TPoint>::value && std::extent<TPoint>::value >= 2>>: std::true_type {};
    
    /**
     * Helper compile-time function to tell whether a type
     * TPoint fit the point concept requirements.
     */
    template <typename TPoint>
    constexpr bool is_point_v() {
        return is_point<TPoint>::value;
    }
    
    /**
     * Returns the x coordinate of a point.
     * @param p - the point whose x coordinate is requested.
     * @return - the x coordinate.
     */
    template <typename TPoint>
    auto x(const TPoint& p) -> decltype(p.x) {
        return p.x;
    }
    
    /**
     * Returns the X coordinate of a point.
     * @param p - the point whose X coordinate is requested.
     * @return - the X coordinate.
     */
    template <typename TPoint>
    auto x(const TPoint& p) -> decltype(p.X) {
        return p.X;
    }
    
    /**
     * Returns the x coordinate of a point.
     * @param p - the point whose x coordinate is requested.
     * @return - the x coordinate.
     */
    template <typename TPoint>
    auto x(const TPoint& p) -> decltype(p.x()) {
        return p.x();
    }
    
    /**
     * Returns the X coordinate of a point.
     * @param p - the point whose X coordinate is requested.
     * @return - the X coordinate.
     */
    template <typename TPoint>
    auto x(const TPoint& p) -> decltype(p.X()) {
        return p.X();
    }
    
    /**
     * Returns the x coordinate of a point.
     * @param p - the point whose x coordinate is requested.
     * @return - the x coordinate.
     */
    template <typename TPoint>
    auto x(const TPoint& p) -> decltype(p[0]) {
        return p[0];
    }
    
    /**
     * Returns the y coordinate of a point.
     * @param p - the point whose y coordinate is requested.
     * @return - the y coordinate.
     */
    template <typename TPoint>
    auto y(const TPoint& p) -> decltype(p.y) {
        return p.y;
    }
    
    /**
     * Returns the Y coordinate of a point.
     * @param p - the point whose Y coordinate is requested.
     * @return - the Y coordinate.
     */
    template <typename TPoint>
    auto y(const TPoint& p) -> decltype(p.Y) {
        return p.Y;
    }
    
    /**
     * Returns the y coordinate of a point.
     * @param p - the point whose y coordinate is requested.
     * @return - the y coordinate.
     */
    template <typename TPoint>
    auto y(const TPoint& p) -> decltype(p.y()) {
        return p.y();
    }
    
    /**
     * Returns the Y coordinate of a point.
     * @param p - the point whose Y coordinate is requested.
     * @return - the Y coordinate.
     */
    template <typename TPoint>
    auto y(const TPoint& p) -> decltype(p.Y()) {
        return p.Y();
    }
    
    /**
     * Returns the y coordinate of a point.
     * @param p - the point whose y coordinate is requested.
     * @return - the y coordinate.
     */
    template <typename TPoint>
    auto y(const TPoint& p) -> decltype(p[1]) {
        return p[1];
    }
}

#endif
