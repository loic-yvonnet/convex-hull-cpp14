#include "test_main.hpp"
#include "point_concept_test.hpp"
#include "angle_test.hpp"
#include "algorithms_test.hpp"

#include <iostream>

int main() {
    run_tests();
    std::cout << "Test suite: OK" << std::endl;
}
