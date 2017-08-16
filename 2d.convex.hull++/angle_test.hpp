#ifndef angle_test_h
#define angle_test_h

#include "test_main.hpp"
#include "angle.hpp"

#include <algorithm>
#include <array>
#include <cmath>

static const double pi = std::acos(-1);

struct dummy_point {
    double x{};
    double y{};
};

bool operator==(const dummy_point& p1, const dummy_point& p2) {
    using namespace hull;
    return equals(x(p1), x(p2)) && equals(y(p1), y(p2));
}

static auto test_angle_with_j = add_test([] {
    // Arrange
    const dummy_point p0{0., 0.};
    const dummy_point p1{1., 1.};
    const dummy_point p2{0., 1.};
    const dummy_point p3{1., 0.};
    const dummy_point p4{-1., -1.};
    
    // Act
    const auto angle0 = hull::get_angle_with_j(p0);
    const auto angle1 = hull::get_angle_with_j(p1);
    const auto angle2 = hull::get_angle_with_j(p2);
    const auto angle3 = hull::get_angle_with_j(p3);
    const auto angle4 = hull::get_angle_with_j(p4);
    
    // Assert
    assert(hull::equals(angle0, 0.));
    assert(hull::equals(angle1, pi/4.));
    assert(hull::equals(angle2, pi/2.));
    assert(hull::equals(angle3, 0.));
    assert(hull::equals(angle4, -pi/2 - pi/4));
});

static auto test_angle_with_j_with_origin = add_test([] {
    // Arrange
    const dummy_point o{-1., -1.};
    const dummy_point p1{0., 0.};
    const dummy_point p2{-1., 0.};
    const dummy_point p3{0., -1.};
    
    // Act
    const auto angle1 = hull::get_angle_with_j(p1, o);
    const auto angle2 = hull::get_angle_with_j(p2, o);
    const auto angle3 = hull::get_angle_with_j(p3, o);
    
    // Assert
    assert(hull::equals(angle1, pi/4.));
    assert(hull::equals(angle2, pi/2.));
    assert(hull::equals(angle3, 0.));
});

static auto test_slow_compare_angles = add_test([] {
    // Arrange
    const dummy_point p1{1., 0.};
    const dummy_point p2{1., 1.};
    const dummy_point p3{0., 1.};
    
    // Act
    const auto p1p2 = hull::slow_compare_angles(p1, p2);
    const auto p1p3 = hull::slow_compare_angles(p1, p3);
    const auto p2p3 = hull::slow_compare_angles(p2, p3);
    const auto p2p1 = hull::slow_compare_angles(p2, p1);

    // Assert
    assert(p1p2);
    assert(p1p3);
    assert(p2p3);
    assert(!p2p1);
});

static auto test_slow_compare_angles_with_origin = add_test([] {
    // Arrange
    const dummy_point o{10., 10.};
    const dummy_point p1{1., 0.};
    const dummy_point p2{1., 1.};
    const dummy_point p3{0., 1.};
    
    // Act
    const auto p1p2 = hull::slow_compare_angles(p1, p2, o);
    const auto p1p3 = hull::slow_compare_angles(p1, p3, o);
    const auto p2p3 = hull::slow_compare_angles(p2, p3, o);
    const auto p2p1 = hull::slow_compare_angles(p2, p1, o);
    
    // Assert
    assert(!p1p2);
    assert(!p1p3);
    assert(!p2p3);
    assert(p2p1);
});

static auto test_slow_sort_angles = add_test([] {
    // Arrange
    auto points = std::array<dummy_point, 10>{{
        {13., 5.}, {12., 8.}, {10., 3.}, {7., 7.},
        {9., 6.}, {4., 0.}, {7., 1.}, {7., 4.},
        {3., 3.}, {1., 1.}
    }};
    const auto expected = std::array<dummy_point, 10>{{
        {4., 0.}, {7., 1.}, {10., 3.}, {13., 5.},
        {7., 4.}, {9., 6.}, {12., 8.}, {1., 1.},
        {3., 3.}, {7., 7.}
    }};
    
    // Act
    std::sort(std::begin(points), std::end(points),
              [](const auto& p1, const auto& p2) {
                  return hull::slow_compare_angles(p1, p2);
              });
    
    // Assert
    assert(std::equal(std::begin(points), std::end(points), std::begin(expected)));
});

static auto test_slow_sort_angles_with_negative_x = add_test([] {
    // Arrange
    auto points = std::array<dummy_point, 12>{{
        {0., 10.}, {-6., 0.}, {7., 2.}, {-5., 5.},
        {6., 5.}, {-2., 3.}, {2., 4.}, {3., 0.},
        {-5., 1.}, {1., 3.}, {-2., 5.}, {4., 2.}
    }};
    const auto expected = std::array<dummy_point, 12>{{
        {3., 0.}, {7., 2.}, {4., 2.}, {6., 5.},
        {2., 4.}, {1., 3.}, {0., 10.}, {-2., 5.},
        {-2., 3.}, {-5., 5.}, {-5., 1.}, {-6., 0.}
    }};
    
    // Act
    std::sort(std::begin(points), std::end(points),
              [](const auto& p1, const auto& p2) {
                  return hull::slow_compare_angles(p1, p2);
              });
    
    // Assert
    assert(std::equal(std::begin(points), std::end(points), std::begin(expected)));
});

static auto test_sort_angles = add_test([] {
    // Arrange
    auto points = std::array<dummy_point, 10>{{
        {13., 5.}, {12., 8.}, {10., 3.}, {7., 7.},
        {9., 6.}, {4., 0.}, {7., 1.}, {7., 4.},
        {3., 3.}, {1., 1.}
    }};
    const auto expected = std::array<dummy_point, 10>{{
        {4., 0.}, {7., 1.}, {10., 3.}, {13., 5.},
        {7., 4.}, {9., 6.}, {12., 8.}, {1., 1.},
        {3., 3.}, {7., 7.}
    }};
    
    // Act
    std::sort(std::begin(points), std::end(points),
              [](const auto& p1, const auto& p2) {
                  return hull::compare_angles(p1, p2);
              });
    
    // Assert
    assert(std::equal(std::begin(points), std::end(points), std::begin(expected)));
});

static auto test_sort_angles_with_negative_x = add_test([] {
    // Arrange
    auto points = std::array<dummy_point, 12>{{
        {0., 10.}, {-6., 0.}, {7., 2.}, {-5., 5.},
        {6., 5.}, {-2., 3.}, {2., 4.}, {3., 0.},
        {-5., 1.}, {1., 3.}, {-2., 5.}, {4., 2.}
    }};
    const auto expected = std::array<dummy_point, 12>{{
        {3., 0.}, {7., 2.}, {4., 2.}, {6., 5.},
        {2., 4.}, {1., 3.}, {0., 10.}, {-2., 5.},
        {-2., 3.}, {-5., 5.}, {-5., 1.}, {-6., 0.}
    }};
    
    // Act
    std::sort(std::begin(points), std::end(points),
              [](const auto& p1, const auto& p2) {
                  return hull::compare_angles(p1, p2);
              });
    
    // Assert
    assert(std::equal(std::begin(points), std::end(points), std::begin(expected)));
});

#endif
