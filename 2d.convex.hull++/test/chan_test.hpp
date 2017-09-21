/**
 * Unit tests for Chan algorithm.
 */

#ifndef chan_test_h
#define chan_test_h

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
    const auto last = hull::algorithms::chan_impl(std::begin(points), std::end(points), std::back_inserter(target), 6);
    
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
    const auto last = hull::algorithms::chan_impl(std::begin(points), std::end(points), std::back_inserter(target), 5);
    
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

#endif
