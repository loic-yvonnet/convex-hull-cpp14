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

static auto test_sort_by_polar_angles = add_test([] {
    // Arrange
    auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 10>{{
        {4, 0}, {7, 1}, {10, 3}, {13, 5},
        {12, 8}, {9, 6}, {7, 4},
        {7, 7}, {3, 3}, {1, 1}
    }};
    
    // Act
    hull::algorithms::details::sort_by_polar_angles(std::begin(points), std::end(points));
    
    // Assert
    assert(std::equal(std::begin(points), std::end(points), std::begin(expected)));
});

static auto test_graham_scan_from_sorted_angles = add_test([] {
    // Arrange
    auto points = std::array<point2d, 10>{{
        {4, 0}, {7, 1}, {10, 3}, {13, 5},
        {12, 8}, {9, 6}, {7, 4},
        {7, 7}, {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 10>{{
        {4, 0}, {7, 1}, {13, 5},
        {12, 8}, {7, 7}, {1, 1}
    }};
    
    // Act
    const auto last = hull::algorithms::details::perform_graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

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
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_with_negative_x = add_test([] {
    // Arrange
    auto points = std::array<point2d, 13>{{
        {0, 10},
        {-5, 5}, {-2, 5}, {2, 4}, {6, 5},
        {-5, 1}, {-2, 3}, {1, 3}, {4, 2}, {7, 2},
        {-3, 0}, {0, 0}, {3, 0}
    }};
    const auto expected = std::array<point2d, 7>{{
        {-3, 0}, {3, 0}, {7, 2}, {6, 5},
        {0, 10}, {-5, 5}, {-5, 1}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_general_case = add_test([] {
    // Arrange
    auto points = std::array<point2d, 23>{{
        {5, 11}, {-3, 10}, {-6, -5}, {14, 11},
        {-5, -14}, {-16, 0}, {2, -14}, {8, -8},
        {-5, 0},  {5, 4}, {-10, 7}, {0, -6},
        {-9, -8}, {17, -9}, {-16, -8}, {10, 8},
        {2, -3}, {0, 14}, {-3, 4}, {11, 0},
        {-12, -12}, {-5, 7}, {-14, -10}
    }};
    const auto expected = std::array<point2d, 9>{{
        {-5, -14}, {2, -14}, {17, -9}, {14, 11}, {0, 14},
        {-10, 7}, {-16, 0}, {-16, -8}, {-12, -12}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_square_case = add_test([] {
    // Arrange
    auto points = std::array<point2d, 12>{{
        {0, 0}, {5, 5}, {5, 0}, {-5, 0},
        {-5, 5}, {-5, -5}, {0, -5}, {0, 5},
        {5, -5}, {2, 3}, {-3, 2}, {-5, 4}
    }};
    const auto expected = std::array<point2d, 4>{{
        {-5, -5}, {5, -5}, {5, 5}, {-5, 5}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_triangle_case = add_test([] {
    // Arrange
    auto points = std::array<point2d, 12>{{
        {0, 0}, {5, -5}, {-5, -5}, {-1, 2},
        {2, 0}, {-2, -1}, {1, 2}, {1, 1},
        {2, -2}, {0, 5}, {-2, 1}, {1, 0}
    }};
    const auto expected = std::array<point2d, 3>{{
        {-5, -5}, {5, -5}, {0, 5}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_with_empty_set = add_test([] {
    // Arrange
    auto points = std::array<point2d, 0>{{}};
    const auto expected = std::array<point2d, 0>{{}};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_with_1_point = add_test([] {
    // Arrange
    auto points = std::array<point2d, 1>{{{0, 0}}};
    const auto expected = std::array<point2d, 1>{{{0, 0}}};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_with_2_points = add_test([] {
    // Arrange
    auto points = std::array<point2d, 2>{{
        {5, 0}, {-2, -3}
    }};
    const auto expected = std::array<point2d, 2>{{
        {-2, -3}, {5, 0}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_with_3_points = add_test([] {
    // Arrange
    auto points = std::array<point2d, 3>{{
        {5, 0}, {0, 0}, {-2, -3}
    }};
    const auto expected = std::array<point2d, 3>{{
        {-2, -3}, {5, 0}, {0, 0}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_collinear_x = add_test([] {
    // Arrange
    auto points = std::array<point2d, 4>{{
        {1, 1}, {-3, 1}, {-10, 1}, {10, 1}
    }};
    const auto expected = std::array<point2d, 2>{{
        {-10, 1}, {10, 1}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_collinear_y = add_test([] {
    // Arrange
    auto points = std::array<point2d, 4>{{
        {1, 1}, {1, -3}, {1, -10}, {1, 10}
    }};
    const auto expected = std::array<point2d, 2>{{
        {1, -10}, {1, 10}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_collinear = add_test([] {
    // Arrange
    auto points = std::array<point2d, 4>{{
        {23, 23}, {1, 1}, {3, 3}, {-7, -7}
    }};
    const auto expected = std::array<point2d, 2>{{
        {-7, -7}, {23, 23}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_sort_by_polar_angles_with_duplicates = add_test([] {
    // Arrange
    auto points = std::array<point2d, 8>{{
        {-6, -6}, {5, -4}, {-6, -6}, {7, 8},
        {-6, -6}, {-6, -6}, {-6, -6}, {-6, -6}
    }};
    const auto expected = std::array<point2d, 10>{{
        {-6, -6}, {-6, -6}, {-6, -6}, {-6, -6},
        {-6, -6}, {-6, -6}, {5, -4}, {7, 8}
    }};
    
    // Act
    hull::algorithms::details::sort_by_polar_angles(std::begin(points), std::end(points));
    
    // Assert
    assert(std::equal(std::begin(points), std::end(points), std::begin(expected)));
});

static auto test_graham_scan_duplicate_origin = add_test([] {
    // Arrange
    auto points = std::array<point2d, 8>{{
        {-6, -6}, {5, -4}, {-6, -6}, {7, 8},
        {-6, -6}, {-6, -6}, {-6, -6}, {-6, -6}
    }};
    const auto expected = std::array<point2d, 3>{{
        {-6, -6}, {5, -4}, {7, 8}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_duplicate_points = add_test([] {
    // Arrange
    auto points = std::array<point2d, 16>{{
        {-5, 7}, {0, 0}, {5, -4}, {2, 3},
        {-3, -2}, {-6, -6}, {7, 8}, {5, 6},
        {7, 8}, {5, 6}, {-3, -2}, {-6, -6},
        {5, -4}, {2, 3}, {-5, 7}, {0, 0}
    }};
    const auto expected = std::array<point2d, 4>{{
        {-6, -6}, {5, -4}, {7, 8}, {-5, 7}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

static auto test_graham_scan_more_duplicate_points = add_test([] {
    // Arrange
    auto points = std::array<point2d, 32>{{
        {-5, 7}, {0, 0}, {5, -4}, {2, 3},
        {-3, -2}, {-6, -6}, {7, 8}, {5, 6},
        {7, 8}, {5, 6}, {-3, -2}, {-6, -6},
        {5, -4}, {2, 3}, {-5, 7}, {0, 0},
        {-5, 7}, {0, 0}, {5, -4}, {2, 3},
        {-3, -2}, {-6, -6}, {7, 8}, {5, 6},
        {7, 8}, {5, 6}, {-3, -2}, {-6, -6},
        {5, -4}, {2, 3}, {-5, 7}, {0, 0}
    }};
    const auto expected = std::array<point2d, 4>{{
        {-6, -6}, {5, -4}, {7, 8}, {-5, 7}
    }};
    
    // Act
    const auto last = hull::algorithms::graham_scan(std::begin(points), std::end(points));
    
    // Assert
    assert(std::distance(std::begin(points), last) == expected.size());
    assert(std::equal(std::begin(points), last, std::begin(expected)));
});

#endif
