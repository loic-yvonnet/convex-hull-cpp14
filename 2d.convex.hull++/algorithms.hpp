#ifndef algorithms_h
#define algorithms_h

#include "point_concept.hpp"
#include "angle.hpp"

#include <algorithm>
#include <iterator>

namespace hull {
    namespace algorithms {
        namespace details {
            template <typename ForwardIt>
            void sort_by_polar_angles(ForwardIt first, ForwardIt last) {
                static_assert(is_point_v<typename std::iterator_traits<ForwardIt>::value_type>(), "ill-formed point");
                static_assert(std::is_base_of<
                                std::forward_iterator_tag,
                                typename std::iterator_traits<ForwardIt>::iterator_category
                              >(), "forward access iterator required");
                
                if (std::distance(first, last) < 2) {
                    return ;
                }
                
                // swap points[1] with the point with the lowest y-coordinate
                {
                    auto lowest_y = std::min_element(first, last, [](const auto& p1, const auto& p2) {
                        return (y(p1) < y(p2) || (hull::equals(y(p1), y(p2)) && x(p1) < x(p2)));
                    });
                    
                    std::swap(*first, *lowest_y);
                }
                
                // sort points by polar angle with points[1]
                std::sort(first+1, last, [origin = *first](const auto& p1, const auto& p2) {
                    return hull::compare_angles(p1, p2, origin);
                });
            }
            
            template <typename RandomIt>
            RandomIt perform_graham_scan(RandomIt first, RandomIt last) {
                // let N = number of points
                auto N = std::distance(first, last);
                if (N <= 3) {
                    return last;
                }
                
                // M will denote the number of points on the convex hull.
                // let M = 1
                std::size_t M{1};
                
                // let points[N+1] = the array of points
                // We want points[0] to be a sentinel point that will stop the loop.
                // let points[0] = points[N]
                auto points = [first, N](std::size_t i) {
                    if (i == 0) {
                        return first + (N - 1);
                    }
                    else {
                        return first + (i - 1);
                    }
                };
                
                // for i = 2 to N:
                for (std::size_t i{2}; i <= N; i++) {
                    // Find next valid point on convex hull.
                    // while ccw(points[M-1], points[M], points[i]) <= 0:
                    while (cross(*points(M-1), *points(M), *points(i)) <= 0) {
                        if (M > 1) {
                            M--;
                            continue;
                        }
                        else if (i == N) {
                            // All points are collinear
                            break;
                        }
                        else {
                            i++;
                        }
                    }
                    
                    // Update M and swap points[i] to the correct place.
                    M++;
                    
                    // swap points[M] with points[i]
                    std::swap(*points(M), *points(i));
                }
                
                return first + M;
            }
        }
        
        // Reference: https://en.wikipedia.org/wiki/Graham_scan
        template <typename RandomIt>
        RandomIt graham_scan(RandomIt first, RandomIt last) {
            static_assert(is_point_v<typename std::iterator_traits<RandomIt>::value_type>(), "ill-formed point");
            static_assert(std::is_same<
                            typename std::iterator_traits<RandomIt>::iterator_category,
                            std::random_access_iterator_tag
                          >(), "random access iterator required");
            
            details::sort_by_polar_angles(first, last);

            return details::perform_graham_scan(first, last);
        }
        
        // Reference: https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
        template <typename RandomIt1, typename RandomIt2>
        RandomIt2 monotone_chain(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
            using point_type1 = typename std::iterator_traits<RandomIt1>::value_type;
            using point_type2 = typename std::iterator_traits<RandomIt2>::value_type;
            
            static_assert(is_point_v<point_type1>(), "ill-formed point");
            static_assert(is_point_v<point_type2>(), "ill-formed point");
            static_assert(std::is_same<
                          typename std::iterator_traits<RandomIt1>::iterator_category,
                          std::random_access_iterator_tag
                          >(), "random access iterator required");
            static_assert(std::is_same<
                          typename std::iterator_traits<RandomIt2>::iterator_category,
                          std::random_access_iterator_tag
                          >(), "random access iterator required");
            
            // Sort the points of P by x-coordinate (in case of a tie, sort by y-coordinate)
            std::sort(first, last, [](const auto& p1, const auto& p2) {
                return (x(p1) < x(p2) || (hull::equals(x(p1), x(p2)) && y(p1) < y(p2)));
            });
            
            const auto N = std::distance(first, last);
            if (N <= 1) {
                return std::copy(first, last, first2);
            }
            
            std::size_t k{};
            
            auto no_counter_clockwise = [&k, first, first2](auto i) {
                return cross(*(first2 + (k - 2)), *(first2 + (k - 1)), *(first + i)) <= 0;
            };
            
            auto copy = [&k, first, first2](auto i) {
                *(first2 + k) = *(first + i);
                k++;
            };
            
            // Lower hull
            for (int i{}; i < N; i++) {
                while (k >= 2 && no_counter_clockwise(i)) {
                    k--;
                }
                copy(i);
            }
            
            // Upper hull
            auto t = k + 1;
            for (int i{static_cast<int>(N - 2)}; i >= 0; i--) {
                while (k >= t && no_counter_clockwise(i)) {
                    k--;
                }
                copy(i);
            }
            
            return first2 + (k - 1);
        }
    }
    
    template <typename ForwardIt, typename RandomIt>
    auto compute_convex_hull(ForwardIt first, ForwardIt last, RandomIt first2) {
        auto last2 = std::copy(first, last, first2);
        return algorithms::graham_scan(first2, last2);
    }
    
    template <typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull2(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        std::fill_n(first2, 2 * std::distance(first, last),
                    std::declval<typename std::iterator_traits<RandomIt2>::value_type>());
        
        return algorithms::monotone_chain(first, last, first2);
    }
}

#endif
