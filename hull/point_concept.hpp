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

#include "reflection.hpp"

#include <array>
#include <type_traits>
#include <tuple>
#include <utility>

namespace hull {
    namespace details {
        /**
         * There is no definition of tuple_size for non-tuple types and we need one so
         * we wrap the definition of tuple_size. The default implementation returns 0.
         */
        template <typename TTuple>
        struct tuple_size {
            static constexpr std::size_t value{};
        };
        
        template <typename... TArgs>
        struct tuple_size<std::tuple<TArgs...>>: std::tuple_size<std::tuple<TArgs...>> {};
        
        template <typename T, typename U>
        struct tuple_size<std::pair<T, U>>: std::tuple_size<std::pair<T, U>> {};
        
        template <typename T, std::size_t N>
        struct tuple_size<std::array<T, N>>: std::tuple_size<std::array<T, N>> {};
        
        template <typename TTuple>
        constexpr auto tuple_size_v = tuple_size<TTuple>::value;
        
        /**
         * Same thing for tuple_element, which has no general definition. We provide one
         * here which returns void.
         */
        template <std::size_t I, typename TTuple>
        struct tuple_element {
            using type = void;
        };
        
        template <std::size_t I, typename... TArgs>
        struct tuple_element<I, std::tuple<TArgs...>>: std::tuple_element<I, std::tuple<TArgs...>> {};
        
        template <std::size_t I, typename T>
        struct tuple_element<I, std::pair<T, T>>: std::tuple_element<I, std::pair<T, T>> {};
        
        template <std::size_t I, typename T, std::size_t N>
        struct tuple_element<I, std::array<T, N>>: std::tuple_element<I, std::array<T, N>> {};
        
        template <std::size_t I, typename... TArgs>
        using tuple_element_t = typename tuple_element<I, TArgs...>::type;
        
        /**
         * Type trait to determine whether a given type is a pair. Note
         * that this implementation also checks that the 2 inner types
         * of the pair are the same.
         */
        template <typename TPair, typename T, typename U>
        struct is_pair: std::false_type {};
        
        template <typename T>
        struct is_pair<std::pair<T, T>, T, T>: std::true_type {};
        
        template <typename TPair>
        constexpr bool is_pair_v() {
            if constexpr(tuple_size_v<TPair> != 2) {
                return false;
            }
            else {
                using T = tuple_element_t<0, TPair>;
                using U = tuple_element_t<1, TPair>;
                return is_pair<TPair, T, U>::value;
            }
        }
        
        /**
         * Type trait to determine whether a given type is a tuple. Note
         * that this implementation also checks that the first 2 inner
         * types of the tuple are the same.
         */
        template <typename TTuple, typename... TArgs>
        struct is_tuple: std::false_type {};
        
        template <typename T, typename... TOthers>
        struct is_tuple<std::tuple<T, T, TOthers...>, T, T, TOthers...>: std::true_type {};
        
        template <typename TTuple, std::size_t... Is>
        constexpr bool is_tuple_v_impl(std::index_sequence<Is...>) {
            return is_tuple<TTuple, tuple_element_t<Is, TTuple>...>::value;
        }
        
        template <typename TTuple, typename... TArgs>
        constexpr bool is_tuple_v() {
            constexpr auto size = tuple_size_v<TTuple>;
            
            using indices = std::make_index_sequence<size>;
            
            return is_tuple_v_impl<TTuple>(indices{});
        }
        
        /**
         * Type trait to determine whether a given type is a std::array.
         * Note that this implementation also checks that there are at
         * least 2 elements in the array.
         */
        template <typename TArray, typename T, std::size_t N>
        struct is_array: std::false_type {};
        
        template <typename T, std::size_t N>
        struct is_array<std::array<T, N>, T, N>: std::conditional_t<N >= 2, std::true_type, std::false_type> {};
        
        template <typename TArray>
        constexpr bool is_array_v() {
            using T = tuple_element_t<0, TArray>;
            constexpr auto N = tuple_size_v<TArray>;
            return is_array<TArray, T, N>::value;
        }
    }
    
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
     * WARNING: the algorithms in this library require also operator= on TPoint.
     * A C-style array does not have operator=. You need to use a C++ std::array.
     */
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<std::is_array<TPoint>::value && std::extent<TPoint>::value >= 2>>: std::true_type {};
    
    /**
     * If the type TPoint is a pair of the same type T, then it fits
     * the point concept requirements.
     */
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<details::is_pair_v<TPoint>()>>: std::true_type {};
    
    /**
     * If the type TPoint is a tuple whose first 2 types are the same, then it fits
     * the point concept requirements.
     */
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<details::is_tuple_v<TPoint>()>>: std::true_type {};
    
    /**
     * If the type TPoint is a std::array of size greater than 2, then it fits
     * the point concept requirements.
     */
    template <typename TPoint>
    struct is_point<TPoint, std::enable_if_t<details::is_array_v<TPoint>()>>: std::true_type {};
    
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
     * Returns the x coordinate of a point.
     * @param p - the point whose x coordinate is requested.
     * @return - the x coordinate.
     */
    template <
        typename TPoint,
        typename = std::enable_if_t<!details::is_array_v<TPoint>()>
    >
    auto x(const TPoint& p) -> decltype(std::get<0>(p)) {
        return std::get<0>(p);
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
    
    /**
     * Returns the y coordinate of a point.
     * @param p - the point whose y coordinate is requested.
     * @return - the y coordinate.
     */
    template <
        typename TPoint,
        typename = std::enable_if_t<!details::is_array_v<TPoint>()>
    >
    auto y(const TPoint& p) -> decltype(std::get<1>(p)) {
        return std::get<1>(p);
    }
}

#endif
