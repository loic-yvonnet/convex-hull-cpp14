#ifndef point_concept_test_h
#define point_concept_test_h

#include <cassert>

#include "test_main.hpp"
#include "point_concept.hpp"

static auto test_has_member_x = add_test([] {
    // Arrange
    struct p1 {
        int x;
    };
    
    struct p2 {
    };
    
    // Act & Assert
    assert(hull::has_member_x<p1>::value == 1);
    assert(hull::has_member_x<p2>::value == 0);
});

static auto test_valid_point_lower_case = add_test([] {
    // Arrange
    struct p {
        int x;
        int y;
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == 1);
});

static auto test_valid_point_upper_case = add_test([] {
    // Arrange
    struct p {
        int X;
        int Y;
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == 1);
});

static auto test_missing_y_coordinate = add_test([] {
    // Arrange
    struct p {
        int x;
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == 0);
});

static auto test_missing_x_coordinate = add_test([] {
    // Arrange
    struct p {
        int y;
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == 0);
});

static auto test_missing_coordinates = add_test([] {
    // Arrange
    struct p {
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == 0);
});

static auto test_x_free_function = add_test([] {
    // Arrange
    struct p {
        int x{};
    } _;
    
    // Act & Assert
    assert(hull::x<p>(_) == 0);
});

static auto test_X_free_function = add_test([] {
    // Arrange
    struct p {
        int X{};
    } _;
    
    // Act & Assert
    assert(hull::x<p>(_) == 0);
});

#endif
