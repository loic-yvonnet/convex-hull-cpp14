// Basic reflection using SFINAE
// References:
//  - https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
//  - https://stackoverflow.com/questions/257288/is-it-possible-to-write-a-template-to-check-for-a-functions-existence

#ifndef reflection_h
#define reflection_h

#include <type_traits>
#include <utility>

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

#endif
