/**
 * This file contains the convex hull computation algorithms.
 * More about convex hull: https://en.wikipedia.org/wiki/Convex_hull
 * For the moment, only Graham Scan and Monotone Chain are
 * implemented.
 * You may refer to the unit tests to see how to use this
 * small library.
 * The provided algorithms are mostly iterator-based, following the
 * STL idioms for generality. However, container-based versions are
 * provided for ease-of-use.
 * Example:
 *      struct point { double x; double y; }
 *      std::vector<point> points = {{5., 2.}, ... };
 *      std::vector<point> convex_hull;
 *      hull::convex::compute(points, convex_hull);
 */

#ifndef algorithms_h
#define algorithms_h

#include "point_concept.hpp"
#include "angle.hpp"

#include <algorithm>
#include <iterator>

namespace hull {
    namespace algorithms {
        namespace details {
            /**
             * Sort a container of points by their polar angles, as described
             * in the Graham Scan algorithm.
             * @param first - forward iterator to the first point of the container.
             * @param last - forward iterator to the one-past last point of the container.
             */
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
            
            /**
             * Perform the selection part of the Graham Scan algorithm on a container
             * of points. This selection occurs in-place.
             * @param first - the random access iterator to the first point of the container.
             * @param last - the random access iterator to the one-past last point of the container.
             * @return - the iterator to the last element forming the convex hull of the
             *           provided container of points.
             */
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
        
        /**
         * Compute the convex hull of a container of points following
         * the Graham Scan algorithm. This algorithm works in-place, and
         * consequently modifies the input points.
         * Average time complexity: O(N log(N)) where N is the number of
         * points.
         * Average space complexity: O(N).
         * Reference: https://en.wikipedia.org/wiki/Graham_scan
         * @param first - the random access iterator to the first point of the container.
         * @param last - the random access iterator to the one-past last point of the container.
         * @return - the iterator to the last element forming the convex hull of the
         *           provided container of points.
         */
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
        
        /**
         * Compute the convex hull of a container of points following
         * the Monotone Chain algorithm. This algorithm does not work in-place, but
         * it still modifies the input points (it sorts them by x-coordinates).
         * Average time complexity: O(N log(N)) where N is the number of
         * points.
         * Average space complexity: O(3*N).
         * Reference: https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
         * @param first - the random access iterator to the first point of the container.
         * @param last - the random access iterator to the one-past last point of the container.
         * @param first2 - the random access iterator to the first point of the destination container.
         * @return - the iterator to the last element forming the convex hull of the
         *           provided container of points.
         */
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
    
    /**
     * Compile-time enumeration to choose the
     * algorithm thanks to a policy approach.
     * @param graham_scan_t - Graham Scan algorithm.
     * @param monotone_chain_t - Monotone Chain algorithm.
     */
    struct graham_scan_t {};
    struct monotone_chain_t {};
    
    /**
     * Algorithms policies to choose an overload.
     * @param graham_scan - Graham Scan algorithm.
     * @param monotone_chain - Monotone Chain algorithm.
     */
    namespace choice {
        static constexpr const graham_scan_t graham_scan{};
        static constexpr const monotone_chain_t monotone_chain{};
    }
    
    /**
     * Overload of iterator-based convex hull computation for Graham Scan.
     * Note that the input is modified and there is an extra copy to a 
     * destination container.
     * Average time complexity: O(N log(N)) where N is the number of
     * points.
     * Average space complexity: O(2*N).
     * It is unfortunately not possible to use a std::back_insert_iterator.
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           destination container of points.
     */
    template <typename ForwardIt, typename RandomIt>
    auto compute_convex_hull(graham_scan_t policy, ForwardIt first, ForwardIt last, RandomIt first2) {
        auto new_last = algorithms::graham_scan(first, last);
        return std::copy(first, new_last, first2);
    }
    
    /**
     * Overload of iterator-based convex hull computation for Monotone Chain.
     * Note that the input is modified.
     * Average time complexity: O(N log(N)) where N is the number of
     * points.
     * Average space complexity: O(3*N).
     * It is unfortunately not possible to use a std::back_insert_iterator.
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           destination container of points.
     */
    template <typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(monotone_chain_t policy, RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        using value_type = typename std::iterator_traits<RandomIt2>::value_type;
        std::fill_n(first2, 2 * std::distance(first, last), value_type{});
        
        return algorithms::monotone_chain(first, last, first2);
    }
    
    /**
     * Policy-based approach to call one of the previous functions.
     */
    template <typename Policy, typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        return compute_convex_hull(Policy{}, first, last, first2);
    }
    
    /**
     * The default policy is Graham Scan because it provides a better
     * average space complexity.
     */
    template <typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        return compute_convex_hull(choice::graham_scan, first, last, first2);
    }
    
    namespace convex {
        /**
         * Overload of container-based convex hull computation for Graham Scan.
         * Average time complexity: O(N log(N)) where N is the number of
         * points.
         * Average space complexity: O(2*N).
         * @param c1 - the input container.
         * @param c2 - the destination container.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(graham_scan_t policy, const TContainer1& c1, TContainer2& c2) {
            c2.resize(c1.size());
            
            std::copy(std::begin(c1), std::end(c1), std::begin(c2));
            auto last = hull::algorithms::graham_scan(std::begin(c2), std::end(c2));
            
            c2.erase(last, std::end(c2));
        }
        
        /**
         * Overload of container-based convex hull computation for Monotone Chain.
         * Average time complexity: O(N log(N)) where N is the number of
         * points.
         * Average space complexity: O(4*N).
         * @param c1 - the input container.
         * @param c2 - the destination container.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(monotone_chain_t policy, TContainer1 c1, TContainer2& c2) {
            c2.resize(2 * c1.size());
            
            std::fill(std::begin(c2), std::end(c2), typename TContainer2::value_type{});
            auto last = hull::algorithms::monotone_chain(std::begin(c1), std::end(c1), std::begin(c2));
            
            c2.erase(last, std::end(c2));
        }
        
        /**
         * Policy-based approach to call one of the previous functions.
         */
        template <typename Policy, typename TContainer1, typename TContainer2>
        void compute(const TContainer1& c1, TContainer2& c2) {
            compute(Policy{}, c1, c2);
        }
        
        /**
         * The default policy is Graham Scan because it provides a better
         * average space complexity.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(const TContainer1& c1, TContainer2& c2) {
            compute<hull::graham_scan_t>(c1, c2);
        }
    }
}

#endif
