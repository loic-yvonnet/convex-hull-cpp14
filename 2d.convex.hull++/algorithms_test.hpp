/**
 * Unit tests for the convex hull algorithms.
 */

#ifndef algorithms_test_h
#define algorithms_test_h

#include "test_main.hpp"
#include "algorithms.hpp"

#include <array>
#include <vector>

/**
 * Simple point structure used to test
 * the convex hull algorithms.
 */
struct point2d {
    int x{};
    int y{};
};

/**
 * Equality operator for std::equal.
 */
bool operator==(const point2d& p1, const point2d& p2) {
    return hull::equals(p1, p2);
}

/**
 * Declares the unit tests for the hull::algorithms namespace.
 */
#include "graham_scan_test.hpp"
#include "monotone_chain_test.hpp"
#include "jarvis_march_test.hpp"

static auto test_convex_hull_compute_with_graham = add_test([] {
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
    const auto last = hull::compute_convex_hull(hull::choice::graham_scan,
                                                std::begin(points), std::end(points), std::begin(target));
    
    // Assert
    assert(std::distance(std::begin(target), last) == expected.size());
    assert(std::equal(std::begin(target), last, std::begin(expected)));
});

static auto test_convex_hull_compute_with_monotone_chain = add_test([] {
    // Arrange
    auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 6>{{
        {1, 1}, {4, 0}, {7, 1},
        {13, 5}, {12, 8}, {7, 7}
    }};
    std::vector<point2d> target(2 * points.size());
    
    // Act
    const auto last = hull::compute_convex_hull(hull::choice::monotone_chain,
                                                std::begin(points), std::end(points), std::begin(target));
    
    // Assert
    assert(std::distance(std::begin(target), last) == expected.size());
    assert(std::equal(std::begin(target), last, std::begin(expected)));
});

static auto test_convex_hull_compute_with_policy = add_test([] {
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
    const auto last = hull::compute_convex_hull<hull::graham_scan_t>(std::begin(points), std::end(points),
                                                                     std::begin(target));
    
    // Assert
    assert(std::distance(std::begin(target), last) == expected.size());
    assert(std::equal(std::begin(target), last, std::begin(expected)));
});

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
    //const auto points = std::array<point2d, 10>{{
    const auto points = std::vector<point2d>{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    };
    //}};
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

static auto test_convex_hull_compute_with_container_with_monotone_chain = add_test([] {
    // Arrange
    const auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 6>{{
        {1, 1}, {4, 0}, {7, 1},
        {13, 5}, {12, 8}, {7, 7},
    }};
    std::vector<point2d> target;
    
    // Act
    hull::convex::compute<hull::monotone_chain_t>(points, target);
    
    // Assert
    assert(target.size() == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

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

#endif
