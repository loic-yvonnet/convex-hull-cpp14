/**
 * Unit tests for Chan algorithm.
 */

#ifndef chan_test_h
#define chan_test_h

#include "test_main.hpp"
#include "../hull/algorithms.hpp"
#include "point2d.hpp"

#include <array>
#include <vector>

static auto test_chan_impl = add_test([] {
    // Arrange
    auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 6>{{
        {4, 0}, {1, 1}, {7, 7},
        {12, 8}, {13, 5}, {7, 1}
    }};
    std::vector<point2d> target;
    
    // Act
    const auto last = hull::algorithms::details::chan_impl(std::begin(points), std::end(points), std::back_inserter(target), 6);
    
    // Assert
    assert(last);
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_impl_with_m_too_low = add_test([] {
    // Arrange
    auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    std::vector<point2d> target;
    
    // Act
    const auto last = hull::algorithms::details::chan_impl(std::begin(points), std::end(points), std::back_inserter(target), 5);
    
    // Assert
    assert(!last);
});

static auto test_chan = add_test([] {
    // Arrange
    auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 6>{{
        {4, 0}, {1, 1}, {7, 7},
        {12, 8}, {13, 5}, {7, 1}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});


static auto test_chan_with_negative_x = add_test([] {
    // Arrange
    auto points = std::array<point2d, 13>{{
        {0, 10},
        {-5, 5}, {-2, 5}, {2, 4}, {6, 5},
        {-5, 1}, {-2, 3}, {1, 3}, {4, 2}, {7, 2},
        {-3, 0}, {0, 0}, {3, 0}
    }};
    const auto expected = std::array<point2d, 7>{{
        {3, 0}, {-3, 0}, {-5, 1},
        {-5, 5}, {0, 10}, {6, 5},
        {7, 2}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_general_case = add_test([] {
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
        {2, -14}, {-5, -14}, {-12, -12},
        {-16, -8}, {-16, 0}, {-10, 7},
        {0, 14}, {14, 11}, {17, -9}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_square_case = add_test([] {
    // Arrange
    auto points = std::array<point2d, 12>{{
        {0, 0}, {5, 5}, {5, 0}, {-5, 0},
        {-5, 5}, {-5, -5}, {0, -5}, {0, 5},
        {5, -5}, {2, 3}, {-3, 2}, {-5, 4}
    }};
    const auto expected = std::array<point2d, 4>{{
        {5, -5}, {-5, -5}, {-5, 5}, {5, 5}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_triangle_case = add_test([] {
    // Arrange
    auto points = std::array<point2d, 12>{{
        {0, 0}, {5, -5}, {-5, -5}, {-1, 2},
        {2, 0}, {-2, -1}, {1, 2}, {1, 1},
        {2, -2}, {0, 5}, {-2, 1}, {1, 0}
    }};
    const auto expected = std::array<point2d, 3>{{
        {5, -5}, {-5, -5}, {0, 5}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_with_empty_set = add_test([] {
    // Arrange
    auto points = std::array<point2d, 0>{{}};
    const auto expected = std::array<point2d, 0>{{}};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_with_1_point = add_test([] {
    // Arrange
    auto points = std::array<point2d, 1>{{{0, 0}}};
    const auto expected = std::array<point2d, 1>{{{0, 0}}};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_with_2_points = add_test([] {
    // Arrange
    auto points = std::array<point2d, 2>{{
        {5, 0}, {-2, -3}
    }};
    const auto expected = std::array<point2d, 2>{{
        {-2, -3}, {5, 0}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_with_3_points = add_test([] {
    // Arrange
    auto points = std::array<point2d, 3>{{
        {5, 0}, {0, 0}, {-2, -3}
    }};
    const auto expected = std::array<point2d, 3>{{
        {-2, -3}, {0, 0}, {5, 0}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_collinear_x = add_test([] {
    // Arrange
    auto points = std::array<point2d, 4>{{
        {1, 1}, {-3, 1}, {-10, 1}, {10, 1}
    }};
    const auto expected = std::array<point2d, 2>{{
        {10, 1}, {-10, 1}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_collinear_y = add_test([] {
    // Arrange
    auto points = std::array<point2d, 4>{{
        {1, 1}, {1, -3}, {1, -10}, {1, 10}
    }};
    const auto expected = std::array<point2d, 2>{{
        {1, -10}, {1, 10}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_collinear = add_test([] {
    // Arrange
    auto points = std::array<point2d, 4>{{
        {23, 23}, {1, 1}, {3, 3}, {-7, -7}
    }};
    const auto expected = std::array<point2d, 2>{{
        {-7, -7}, {23, 23}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_duplicate_origin = add_test([] {
    // Arrange
    auto points = std::array<point2d, 8>{{
        {-6, -6}, {5, -4}, {-6, -6}, {7, 8},
        {-6, -6}, {-6, -6}, {-6, -6}, {-6, -6}
    }};
    const auto expected = std::array<point2d, 3>{{
        {-6, -6}, {7, 8}, {5, -4}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_duplicate_points = add_test([] {
    // Arrange
    auto points = std::array<point2d, 16>{{
        {-5, 7}, {0, 0}, {5, -4}, {2, 3},
        {-3, -2}, {-6, -6}, {7, 8}, {5, 6},
        {7, 8}, {5, 6}, {-3, -2}, {-6, -6},
        {5, -4}, {2, 3}, {-5, 7}, {0, 0}
    }};
    const auto expected = std::array<point2d, 4>{{
        {-6, -6}, {-5, 7}, {7, 8}, {5, -4}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_chan_more_duplicate_points = add_test([] {
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
        {-6, -6}, {-5, 7}, {7, 8}, {5, -4}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::algorithms::chan(std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_convex_hull_compute_with_chan = add_test([] {
    // Arrange
    auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 6>{{
        {4, 0}, {1, 1}, {7, 7},
        {12, 8}, {13, 5}, {7, 1}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::compute_convex_hull(hull::choice::chan,
                              std::begin(points), std::end(points), std::back_inserter(target));
    
    // Assert
    assert(std::distance(std::begin(target), std::end(target)) == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

static auto test_convex_hull_compute_with_container_with_chan = add_test([] {
    // Arrange
    const auto points = std::array<point2d, 10>{{
        {13, 5}, {12, 8}, {10, 3}, {7, 7},
        {9, 6}, {4, 0}, {7, 1}, {7, 4},
        {3, 3}, {1, 1}
    }};
    const auto expected = std::array<point2d, 6>{{
        {4, 0}, {1, 1}, {7, 7},
        {12, 8}, {13, 5}, {7, 1}
    }};
    std::vector<point2d> target;
    
    // Act
    hull::convex::compute<hull::chan_t>(points, target);
    
    // Assert
    assert(target.size() == expected.size());
    assert(std::equal(std::begin(target), std::end(target), std::begin(expected)));
});

#endif
