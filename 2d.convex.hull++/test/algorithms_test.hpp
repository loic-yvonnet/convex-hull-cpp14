/**
 * Unit tests for the convex hull algorithms.
 */

#ifndef algorithms_test_h
#define algorithms_test_h

#include "test_main.hpp"
#include "../hull/algorithms.hpp"
#include "point2d.hpp"

#include <array>
#include <vector>

static auto test_convex_hull_compute_without_policy = add_test([] {
    // Arrange
    auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 6>{{
        {4, 0}, {7, 1}, {13, 5},
        {12, 8}, {7, 7}, {1, 1}
    }};
    std::vector<point2d> target(points.size());
    
    // Act
    const auto last = hull::compute_convex_hull(std::begin(points), std::end(points),
                                                std::begin(target));
    
    // Assert
    assert(std::distance(std::begin(target), last) == expected.size());
    assert(std::equal(std::begin(target), last, std::begin(expected)));
});

static auto test_convex_hull_compute_with_container = add_test([] {
    // Arrange
    const auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 6>{{
        {4, 0}, {7, 1}, {13, 5},
        {12, 8}, {7, 7}, {1, 1}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::convex::compute(points, target);
    
    // Assert
    assert(target.size() == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

#endif
