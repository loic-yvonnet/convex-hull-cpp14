/**
 * Unit tests for the point concept.
 */

#include "test_main.hpp"
#include "../hull/point_concept.hpp"

#include <array>

static auto test_has_member_x = add_test([] {
    // Arrange
    struct p1 {
        int x;
    };
    
    struct p2 {
    };
    
    // Act & Assert
    assert(hull::has_member_x<p1>::value == true);
    assert(hull::has_member_x<p2>::value == false);
});

static auto test_valid_point_lower_case = add_test([] {
    // Arrange
    struct p {
        int x;
        int y;
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_valid_point_upper_case = add_test([] {
    // Arrange
    struct p {
        int X;
        int Y;
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_valid_point_method_accessor_lower_case = add_test([] {
    // Arrange
    class p {
        float coord_x{};
        float coord_y{};
    
    public:
        float x() const noexcept { return coord_x; }
        float y() const noexcept { return coord_y; }
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_valid_point_method_accessor_upper_case = add_test([] {
    // Arrange
    class p {
        short coord_x{};
        short coord_y{};
        
    public:
        short X() const noexcept { return coord_x; }
        short Y() const noexcept { return coord_y; }
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_valid_point_array_size_2 = add_test([] {
    // Arrange
    using p = int[2];
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_valid_point_array_size_3 = add_test([] {
    // Arrange
    using p = int[3];
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_valid_point_pair = add_test([] {
    // Arrange
    using p = std::pair<int, int>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_invalid_point_pair = add_test([] {
    // Arrange
    using p = std::pair<int, float>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_valid_point_tuple = add_test([] {
    // Arrange
    using p = std::tuple<float, float>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_valid_point_other_tuple = add_test([] {
    // Arrange
    using p = std::tuple<short, short, char, double, std::pair<int, int>>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_invalid_point_tuple = add_test([] {
    // Arrange
    using p = std::tuple<int, float>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_invalid_point_other_tuple = add_test([] {
    // Arrange
    using p = std::tuple<short>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_invalid_point_another_tuple = add_test([] {
    // Arrange
    using p = std::tuple<char, short, int, long>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_valid_point_array = add_test([] {
    // Arrange
    using p = std::array<int, 3>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_valid_point_other_array = add_test([] {
    // Arrange
    using p = std::array<float, 2>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == true);
});

static auto test_invalid_point_array = add_test([] {
    // Arrange
    using p = std::array<int, 1>;
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_missing_y_coordinate = add_test([] {
    // Arrange
    struct p {
        int x;
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_missing_x_coordinate = add_test([] {
    // Arrange
    struct p {
        int y;
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_missing_coordinates = add_test([] {
    // Arrange
    struct p {
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_case_mismatch = add_test([] {
    // Arrange
    class p {
        short coord_x{};
        short coord_y{};
        
    public:
        short x() const noexcept { return coord_x; }
        short Y() const noexcept { return coord_y; }
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_missing_y_method = add_test([] {
    // Arrange
    class p {
        short coord_x{};
        
    public:
        short x() const noexcept { return coord_x; }
    };
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_too_small_array = add_test([] {
    // Arrange
    using p = float[1];
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_a_value_is_not_a_point = add_test([] {
    // Arrange
    using p = float;
    
    // Act & Assert
    assert(hull::is_point<p>::value == false);
});

static auto test_x_free_function = add_test([] {
    // Arrange
    struct p {
        int x{};
    } _;
    
    // Act & Assert
    assert(hull::x(_) == 0);
});

static auto test_X_free_function = add_test([] {
    // Arrange
    struct p {
        int X{};
    } _;
    
    // Act & Assert
    assert(hull::x(_) == 0);
});

static auto test_x_free_function_for_method_accessor = add_test([] {
    // Arrange
    class p {
        int coord_x{};
        
    public:
        constexpr int x() const noexcept { return coord_x; }
    } _;
    
    // Act & Assert
    assert(hull::x(_) == 0);
});

static auto test_X_free_function_for_method_accessor = add_test([] {
    // Arrange
    class p {
        int coord_x{};
        
    public:
        constexpr int X() const noexcept { return coord_x; }
    } _;
    
    // Act & Assert
    assert(hull::x(_) == 0);
});

static auto test_x_free_function_for_array = add_test([] {
    // Arrange
    char coord[1] = { 0 };
    
    // Act & Assert
    assert(hull::x(coord) == 0);
});

static auto test_y_free_function_for_array = add_test([] {
    // Arrange
    double coord[3] = { 1., 2., 3. };
    
    // Act & Assert
    assert(hull::y(coord) == 2.);
});

static auto test_x_free_function_for_pair = add_test([] {
    // Arrange
    std::pair<int, int> p{};
    
    // Act & Assert
    assert(hull::x(p) == 0);
});

static auto test_y_free_function_for_pair = add_test([] {
    // Arrange
    std::pair<double, double> p{2., 4.};
    
    // Act & Assert
    assert(hull::y(p) == 4.);
});

static auto test_x_free_function_for_tuple = add_test([] {
    // Arrange
    std::tuple<int, int> p{};
    
    // Act & Assert
    assert(hull::x(p) == 0);
});

static auto test_y_free_function_for_tuple = add_test([] {
    // Arrange
    std::tuple<double, double, char> p{2., 4.};
    
    // Act & Assert
    assert(hull::y(p) == 4.);
});

static auto test_x_free_function_for_std_array = add_test([] {
    // Arrange
    std::array<int, 2> p{};
    
    // Act & Assert
    assert(hull::x(p) == 0);
});

static auto test_y_free_function_for_std_array = add_test([] {
    // Arrange
    std::array<int, 2> p{{4, 2}};
    
    // Act & Assert
    assert(hull::y(p) == 2);
});

static auto test_make_point_general_case = add_test([] {
    // Arrange
    using p = std::pair<int, int>;
    
    // Act & Assert
    assert((hull::make_point<p>(4, 2) == p{4, 2}));
});

static auto test_make_point_std_array_case = add_test([] {
    // Arrange
    using p = std::array<int, 2>;
    
    // Act & Assert
    assert((hull::make_point<p>(4, 2) == p{{4, 2}}));
});
