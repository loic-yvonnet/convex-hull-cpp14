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
#include <cmath>
#include <iterator>
#include <limits>
#include <experimental/optional>
#include <utility>

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
         * Average time complexity: O(N * log(N)) where N is the number of
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
         * Average time complexity: O(N * log(N)) where N is the number of
         * points.
         * Average space complexity: O(3 * N).
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
        
        /**
         * Compute the convex hull of a container of points following
         * the Jarvis March algorithm.
         * Average time complexity: O(N * H) where N is the number of input
         * points and H the number of points on the convex hull.
         * Average space complexity: O(N + H).
         * Reference: https://en.wikipedia.org/wiki/Gift_wrapping_algorithm
         * @param first - the random access iterator to the first point of the container.
         * @param last - the random access iterator to the one-past last point of the container.
         * @param first2 - the random access iterator to the first point of the destination container.
         * @return - the iterator to the last element forming the convex hull of the
         *           provided container of points.
         */
        template <typename RandomIt1, typename RandomIt2>
        RandomIt2 jarvis_march(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
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
         
            const auto N = std::distance(first, last);
            if (N <= 1) {
                return std::copy(first, last, first2);
            }
            
            // leftmost point
            const auto min_x = std::min_element(first, last, [](const auto& p1, const auto& p2) {
                return x(p1) < x(p2) || (x(p1) == x(p2) && y(p1) > y(p2));
            });
            auto point_on_hull = *min_x;
            decltype(point_on_hull) endpoint;
            
            // Helper lambda to determine wheter sj is on the left of line pi to endpoint
            auto is_on_the_left = [&endpoint](const auto& pi, const auto& sj) {
                const auto res = cross(pi, endpoint, sj);
                
                // If they are collinear, we take the farthest point from pi
                if (hull::equals(res, static_cast<decltype(res)>(0))) {
                    return hull::square_norm(sj - pi) > hull::square_norm(endpoint - pi);
                }
                
                return res > 0;
            };
            
            // Repeat until wrapped around to first hull point
            std::size_t i{};
            do {
                // P[i] = pointOnHull
                *(first2 + i) = point_on_hull;
                
                // initial endpoint for a candidate edge on the hull
                endpoint = *first2;
                
                // for j from 1 to |S|
                std::for_each(first, last, [&endpoint, &point_on_hull, is_on_the_left](const auto& sj) {
                    // if (endpoint == pointOnHull) or (S[j] is on left of line from P[i] to endpoint)
                    if (hull::equals(endpoint, point_on_hull) || is_on_the_left(point_on_hull, sj)) {
                        // found greater left turn, update endpoint
                        endpoint = sj;
                    }
                });
                
                i++;
                point_on_hull = endpoint;
            }
            while (!hull::equals(endpoint, *first2));
            
            return first2 + i;
        }
        
        /**
         *
         */
        template <
            typename RandomIt,
            typename TPoint = typename RandomIt::value_type
        >
        TPoint max_jarvis_march(RandomIt first, RandomIt last, const TPoint& point_on_hull) {
            auto endpoint = point_on_hull;
            
            // Helper lambda to determine wheter sj is on the left of line pi to endpoint
            auto is_on_the_left = [&endpoint, &point_on_hull](const auto& sj) {
                const auto res = cross(point_on_hull, endpoint, sj);
                
                // If they are collinear, we take the farthest point from pi
                if (hull::equals(res, static_cast<decltype(res)>(0))) {
                    return hull::square_norm(sj - point_on_hull) > hull::square_norm(endpoint - point_on_hull);
                }
                
                return res > 0;
            };
            
            std::for_each(first, last, [&endpoint, &point_on_hull, is_on_the_left](const auto& sj) {
                // if (endpoint == pointOnHull) or (S[j] is on left of line from P[i] to endpoint)
                if (hull::equals(endpoint, point_on_hull) || is_on_the_left(sj)) {
                    // found greater left turn, update endpoint
                    endpoint = sj;
                }
            });
            
            return endpoint;
        }
        
        /**
         * AABB bounding box implementation.
         * This is not a convex hull algorithm but like convex hull, it
         * surrounds the input set of points.
         * The bounding box points are returned counter-clockwise from the
         * bottom-left side.
         * Average time complexity: O(N) where N is the number of points.
         * Average space complexity: O(N).
         * @param first - the forward iterator to the first point of the container.
         * @param last - the forward iterator to the one-past last point of the container.
         * @param first2 - the forward iterator to the first point of the destination container.
         * @return - the iterator to the last element forming the bounding box of the
         *           provided container of points.
         */
        template <typename ForwardIt1, typename ForwardIt2>
        ForwardIt2 bounding_box(ForwardIt1 first, ForwardIt1 last, ForwardIt2 first2) {
            using point_type = typename std::iterator_traits<ForwardIt1>::value_type;
            
            static_assert(is_point_v<point_type>(), "ill-formed point");
            static_assert(std::is_base_of<
                            std::forward_iterator_tag,
                            typename std::iterator_traits<ForwardIt1>::iterator_category
                          >(), "forward access iterator required");
            
            auto less_x = [](const auto& p1, const auto& p2) {
                return x(p1) < x(p2);
            };
            auto less_y = [](const auto& p1, const auto& p2) {
                return y(p1) < y(p2);
            };
            
            const auto [min_x, max_x] = std::minmax_element(first, last, less_x);
            const auto [min_y, max_y] = std::minmax_element(first, last, less_y);
            
            *first2++ = point_type{x(*min_x), y(*min_y)};
            *first2++ = point_type{x(*max_x), y(*min_y)};
            *first2++ = point_type{x(*max_x), y(*max_y)};
            *first2   = point_type{x(*min_x), y(*max_y)};
            
            return first2;
        }
        
        /**
         * Implementation of Chan algorithm, which is theoretically
         * the most performant in operational research terms.
         * Reference: http://www.cs.wustl.edu/~pless/506/l3.html
         */
        template <typename RandomIt, typename OutputIt>
        std::experimental::optional<OutputIt> chan_impl(RandomIt first, RandomIt last, OutputIt first2, std::size_t m) {
            if (first == last) {
                return {};
            }
            
            // (1) Let r = ceil(n/m).
            const auto n = std::distance(first, last);
            const auto r = static_cast<std::size_t>(std::ceil(static_cast<double>(n) / static_cast<double>(m)));
            
            // Partition P into disjoint subsets P(1),P(2),... P(r), each of size at most m.
            // no-op: we take the points in their original order, and assume the subsets at
            // indices [0 ; m - 1], [m ; 2m - 1], etc.
            // We define a lambda to clarify the access to P(i):
            auto P = [first, m](auto i) {
                return first + (i * m);
            };
            
            // Extra memory required to store the size of the sub-convex hulls
            std::vector<RandomIt> lasts;
            lasts.reserve(r);
            
            // (2) For i = 1 to r do:
            //     (a) Compute Hull(P(i)) using Graham's scan and store the vertices in an ordered array.
            for (std::size_t i{}; i < r - 1; i++) {
                const auto convex_hull_last = graham_scan(P(i), P(i + 1));
                lasts.push_back(convex_hull_last);
            }
            
            // Last subset, which may be smaller
            {
                const auto convex_hull_last = graham_scan(P(r - 1), last);
                lasts.push_back(convex_hull_last);
            }
            
            // (3) Let point_on_hull be the bottommost point of P
            const auto min_y = std::min_element(first, last, [](const auto& p1, const auto& p2) {
                return y(p1) < y(p2) || (y(p1) == y(p2) && x(p1) > x(p2));
            });
            auto point_on_hull = *min_y;
            const auto pfirst = *min_y;
            
            // Let endpoint = (-Inf; 0)
            using point_type = decltype(point_on_hull);
            using coord_type = decltype(x(std::declval<point_type>()));
            auto endpoint = point_type{std::numeric_limits<coord_type>::lowest(), static_cast<coord_type>(0)};
            
            // (4) For k = 1 to m do:
            //     (a) For i = 1 to r do:
            //         Compute point q in P(i) that maximizes the angle p(k-1)  p(k)  q
            //     (b) Let p(k+1) be the point q in q(1),q(2),...q(r) than maximizes the angle p(k-1)  p(k)  q
            //     (c) If p(k+1) = p(1) then return {p(1), p(2), ... p(k)}.
            std::vector<point_type> q(r);
            for (std::size_t k{}; k < m; k++) {
                *first2++ = point_on_hull;
                
                for (std::size_t i{}; i < r; i++) {
                    q[i] = max_jarvis_march(P(i), lasts[i], point_on_hull/*, endpoint*/);
                }
                
                const auto pk = max_jarvis_march(std::begin(q), std::end(q), point_on_hull/*, endpoint*/);
                
                if (hull::equals(pk, pfirst)) {
                    return first2;
                }
                
                endpoint = point_on_hull;
                point_on_hull = pk;
            }
            
            // (5) Return "m was too small, try again"
            return {};
        }
        
        template <typename RandomIt, typename OutputIt>
        OutputIt chan(RandomIt first, RandomIt last, OutputIt first2) {
            using point_type = typename std::iterator_traits<RandomIt>::value_type;
            
            static_assert(is_point_v<point_type>(), "ill-formed point");
            static_assert(std::is_same<
                              typename std::iterator_traits<RandomIt>::iterator_category,
                              std::random_access_iterator_tag
                          >(), "random access iterator required");
            
            if (first == last) {
                return first2;
            }
            
            // (1) For t = 1; 2; 3... do:
            //     (a) Let m = min(2^(2^t),n)
            //     (b) Invoke PartialHull(P, m), returning the result in L.
            //     (c) If L != ``try again'' then return L.
            const std::size_t n = std::distance(first, last);
            std::vector<point_type> intermediary(n);
            
            for (std::size_t t{1}; ; t++) {
                const std::size_t pow = 1 << (1 << t);
                const auto m = std::min(pow, n); // warning: may overflow
                const auto last_intermediary = chan_impl(first, last, std::begin(intermediary), m);
                if (last_intermediary) {
                    return std::move(std::begin(intermediary), *last_intermediary, first2);
                }
                intermediary.clear();
            }
        }
    }
    
    /**
     * Compile-time enumeration to choose the
     * algorithm thanks to a policy approach.
     * @param graham_scan_t - Graham Scan algorithm.
     * @param monotone_chain_t - Monotone Chain algorithm.
     * @param jarvis_march_t - Jarvis March algorithm.
     * @param chan_t - Chan's algorithm.
     */
    struct graham_scan_t {};
    struct monotone_chain_t {};
    struct jarvis_march_t {};
    struct chan_t {};
    
    /**
     * Algorithms policies to choose an overload.
     * @param graham_scan - Graham Scan algorithm.
     * @param monotone_chain - Monotone Chain algorithm.
     * @param jarvis_march - Jarvis March algorithm.
     * @param chan - Chan's algorithm.
     */
    namespace choice {
        static constexpr const graham_scan_t graham_scan{};
        static constexpr const monotone_chain_t monotone_chain{};
        static constexpr const jarvis_march_t jarvis_march{};
        static constexpr const chan_t chan{};
    }
    
    /**
     * Overload of iterator-based convex hull computation for Graham Scan.
     * Note that the input is modified and there is an extra copy to a 
     * destination container.
     * Average time complexity: O(N * log(N)) where N is the number of
     * points.
     * Average space complexity: O(2 * N).
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
     * Average time complexity: O(N * log(N)) where N is the number of
     * points.
     * Average space complexity: O(3 * N).
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
     * Overload of iterator-based convex hull computation for Jarvis March.
     * Average time complexity: O(N * H) where N is the number of input
     * points and H is the number of points on the convex hull.
     * Average space complexity: O(2 * N).
     * It is unfortunately not possible to use a std::back_insert_iterator.
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           destination container of points.
     */
    template <typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(jarvis_march_t policy, RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        using value_type = typename std::iterator_traits<RandomIt2>::value_type;
        std::fill_n(first2, std::distance(first, last), value_type{});
        
        return algorithms::jarvis_march(first, last, first2);
    }
    
    /**
     * Overload of iterator-based convex hull computation for Chan.
     * Average time complexity: O(N * log(H)) where N is the number of input
     * points and H is the number of points on the convex hull.
     * Average space complexity: O(3 * N).
     * @param first - the random access iterator to the first point of the container.
     * @param last - the random access iterator to the one-past last point of the container.
     * @param first2 - the random access iterator to the first point of the destination container.
     * @return - the iterator to the last element forming the convex hull of the
     *           destination container of points.
     */
    template <typename RandomIt, typename OutputIt>
    auto compute_convex_hull(chan_t policy, RandomIt first, RandomIt last, OutputIt first2) {
        return algorithms::chan(first, last, first2);
    }
    
    /**
     * Policy-based approach to call one of the previous functions.
     */
    template <typename Policy, typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        return compute_convex_hull(Policy{}, first, last, first2);
    }
    
    /**
     * The default policy is Graham Scan for the moment. A benchmark
     * is required between Graham Scan and Chan's algorithm. In theory,
     * Chan's algorithm is better. However, the practical implementation
     * of Chan's algorithm requires allocations and more space. Therefore,
     * on current computer architectures, Graham Scan is likely faster.
     */
    template <typename RandomIt1, typename RandomIt2>
    auto compute_convex_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2) {
        return compute_convex_hull(choice::graham_scan, first, last, first2);
    }
    
    namespace convex {
        /**
         * Overload of container-based convex hull computation for Graham Scan.
         * Average time complexity: O(N * log(N)) where N is the number of
         * points.
         * Average space complexity: O(2 * N).
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
         * Average time complexity: O(N * log(N)) where N is the number of
         * points.
         * Average space complexity: O(4 * N).
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
         * Overload of container-based convex hull computation for Jarvis March.
         * Average time complexity: O(N * H) where N is the number of input
         * points and H is the number of points on the convex hull.
         * Average space complexity: O(2 * N).
         * @param c1 - the input container.
         * @param c2 - the destination container.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(jarvis_march_t policy, const TContainer1& c1, TContainer2& c2) {
            c2.resize(c1.size());
            
            std::fill(std::begin(c2), std::end(c2), typename TContainer2::value_type{});
            auto last = hull::algorithms::jarvis_march(std::begin(c1), std::end(c1), std::begin(c2));
            
            c2.erase(last, std::end(c2));
        }
        
        /**
         * Overload of container-based convex hull computation for Chan.
         * Average time complexity: O(N * log(H)) where N is the number of input
         * points and H is the number of points on the convex hull.
         * Average space complexity: O(3 * N).
         * @param c1 - the input container.
         * @param c2 - the destination container.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(chan_t policy, TContainer1 c1, TContainer2& c2) {
            hull::algorithms::chan(std::begin(c1), std::end(c1), std::back_inserter(c2));
        }
        
        /**
         * Policy-based approach to call one of the previous functions.
         */
        template <typename Policy, typename TContainer1, typename TContainer2>
        void compute(const TContainer1& c1, TContainer2& c2) {
            compute(Policy{}, c1, c2);
        }
        
        /**
         * The default policy is Graham Scan.
         */
        template <typename TContainer1, typename TContainer2>
        void compute(const TContainer1& c1, TContainer2& c2) {
            compute<hull::graham_scan_t>(c1, c2);
        }
    }
}

#endif
