#ifndef algorithms_test_h
#define algorithms_test_h

#include "test_main.hpp"
#include "algorithms.hpp"

#include <array>

struct point2d {
    int x{};
    int y{};
};

bool operator==(const point2d& p1, const point2d& p2) {
    using namespace hull;
    return x(p1) == x(p2) && y(p1) == y(p2);
}

static auto test_graham_scan = add_test([] {
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
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == 6);
    //assert(std::equal(std::begin(points), last, std::begin(expected)));
});

#endif
