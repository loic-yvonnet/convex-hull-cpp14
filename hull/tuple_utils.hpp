/**
 * Compile-time determination of whether a given type
 * is a tuple, pair or array.
 */

#ifndef tuple_utils_h
#define tuple_utils_h

#include <array>
#include <type_traits>
#include <tuple>
#include <utility>

namespace hull::details {
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

#endif
