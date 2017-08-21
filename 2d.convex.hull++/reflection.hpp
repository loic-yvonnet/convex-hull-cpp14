/**
 * Basic reflection facilities
 * Reflection is not part of C++14/17. However, C++11/14 introduced decltype
 * and auto facilities. These facilities simplify and tidy up the SFINAE-based
 * techniques to query and introspect a type at compile-time.
 * The following implementation benefits from these facilities.
 * References:
 *  - https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
 *  - https://stackoverflow.com/questions/257288/is-it-possible-to-write-a-template-to-check-for-a-functions-existence
 */

#ifndef reflection_h
#define reflection_h

#include <type_traits>
#include <utility>

/**
 * The HAS_MEMBER macro generates 2 overloads of a template function nested in the
 * details::has_member::#member namespace. These functions' overloads have no
 * definition since they are never called at runtime. The 1st overload checks 
 * whether the #member exists in the definition of its template parameter. If so, 
 * this overload is used for overload resolution. This overload returns true. 
 * Otherwise, if the #member does not exist in the definition of the template
 * parameter, the other overload is used for overload resolution. This second
 * overload returns false.
 * The HAS_MEMBER macro generates a template structure which inherits from the
 * result of the compile-time overload resolution of a call to the above test
 * function. Therefore, if the template parameter has the requested member, the
 * structure inherits from true_type, and false_type otherwise.
 * This is how we query for the existence of a data member within a type T.
 */
#define HAS_MEMBER(member) \
\
namespace details { \
    namespace has_member { \
        namespace member { \
            template <typename T> \
            static auto test(int) -> decltype(std::declval<T>().member, std::true_type()); \
            \
            template <typename> \
            static auto test(long) -> std::false_type; \
        } \
    } \
} \
\
template<class T> \
struct has_member_##member : decltype(details::has_member::member::test<T>(0)) {};

/**
 * Exactly the same idea for methods
 */
#define HAS_METHOD(method) \
\
namespace details { \
    namespace has_method { \
        namespace method { \
            template <typename T> \
            static auto test(int) -> decltype(std::declval<T>().method(), std::true_type()); \
            \
            template <typename> \
            static auto test(long) -> std::false_type; \
        } \
    } \
} \
\
template<class T> \
struct has_method_##method : decltype(details::has_method::method::test<T>(0)) {};

#endif
