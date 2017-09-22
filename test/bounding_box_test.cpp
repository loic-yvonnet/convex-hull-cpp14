/**
 * Unit tests for the convex hull algorithms.
 */

#include "test_main.hpp"
#include "../hull/algorithms.hpp"
#include "point2d.hpp"

#include <array>
#include <vector>

static auto test_bounding_box = add_test([] {
    // Arrange
    const auto points = std::array<point2d, 10>{{
        {13, 5}, {-12, 8}, {10, 3}, {7, -7},
        {-9, -6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {-1, 1}
    }};
    const auto expected = std::array<point2d, 4>{{
        {-12, -7}, {13, -7}, {13, 8}, {-12, 8}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::bounding_box(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(target.size() == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});
