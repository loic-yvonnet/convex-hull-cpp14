/**
 * Implementation of Graham Scan for convex hull
 * computation of a set of points in the 2d space.
 */

#ifndef graham_scan_h
#define graham_scan_h

#include "angle.hpp"
#include "assert.hpp"
#include "point_concept.hpp"

#include <algorithm>

namespace hull::algorithms::details {
    /**
     * Sort a container of points by their polar angles, as described
     * in the Graham Scan algorithm.
     * @param first - forward iterator to the first point of the container.
     * @param last - forward iterator to the one-past last point of the container.
     */
    template <typename ForwardIt>
    void sort_by_polar_angles(ForwardIt first, ForwardIt last) {
        static_assert_is_forward_iterator_to_point<ForwardIt>();
        
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

namespace hull::algorithms {
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
        static_assert_is_random_access_iterator_to_point<RandomIt>();
        
        details::sort_by_polar_angles(first, last);
        
        return details::perform_graham_scan(first, last);
    }
}

namespace hull {
    /**
     * Compile-time enumeration to choose the
     * algorithm thanks to a policy approach.
     * @param graham_scan_t - Graham Scan algorithm.
     */
    struct graham_scan_t {};
    
    /**
     * Algorithms policies to choose an overload.
     * @param graham_scan - Graham Scan algorithm.
     */
    namespace choice {
        static constexpr const graham_scan_t graham_scan{};
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
    }
}

#endif
