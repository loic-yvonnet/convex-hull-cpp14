/**
 * Common static assertions gathered here.
 */

#ifndef hull_static_assert_h
#define hull_static_assert_h

#include "point_concept.hpp"

#include <iterator>

namespace hull {
    /**
     * Syntactic sugar to cross-check at compile-time
     * that the TPoint type fits the Point concept.
     */
    template <typename TPoint>
    constexpr void static_assert_is_point() {
        static_assert(is_point_v<TPoint>(), "ill-formed point");
    }
    
    /**
     * Syntactic sugar to cross-check at compile-time
     * that a given Iterator type points to a type which
     * fits the Point concept.
     */
    template <typename Iterator>
    constexpr void static_assert_is_iterator_to_point() {
        using point_type = typename std::iterator_traits<Iterator>::value_type;
        static_assert_is_point<point_type>();
    }
    
    /**
     * Syntactic sugar to cross-check at compile-time
     * that the RandomIt type fits the RandomAccessIterator concept.
     */
    template <typename RandomIt>
    constexpr void static_assert_is_random_access_iterator() {
        static_assert(std::is_same<
                          typename std::iterator_traits<RandomIt>::iterator_category,
                          std::random_access_iterator_tag
                      >(), "random access iterator required");
    }
    
    /**
     * Syntactic sugar to cross-check at compile-time
     * that the ForwardIt type fits the ForwardIterator concept.
     */
    template <typename ForwardIt>
    constexpr void static_assert_is_forward_iterator() {
        static_assert(std::is_base_of<
                          std::forward_iterator_tag,
                          typename std::iterator_traits<ForwardIt>::iterator_category
                      >(), "forward access iterator required");
    }
    
    /**
     * Syntactic sugar to cross-check at compile-time
     * that the RandomIt type fits the RandomAccessIterator concept
     * and points to a type which fits the the Point concept.
     */
    template <typename RandomIt>
    constexpr void static_assert_is_random_access_iterator_to_point() {
        static_assert_is_random_access_iterator<RandomIt>();
        static_assert_is_iterator_to_point<RandomIt>();
    }
    
    /**
     * Syntactic sugar to cross-check at compile-time
     * that the ForwardIt type fits the ForwardIterator concept
     * and points to a type which fits the the Point concept.
     */
    template <typename ForwardIt>
    constexpr void static_assert_is_forward_iterator_to_point() {
        static_assert_is_forward_iterator<ForwardIt>();
        static_assert_is_iterator_to_point<ForwardIt>();
    }
}

#endif

