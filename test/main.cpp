/**
 * The main function simply runs all the unit tests.
 * If a unit test fails, it stops immediately the process.
 * If all unit tests pass, a success message is printed
 * to the standard output.
 */

#include "test_main.hpp"

#include <iostream>

int main() {
    run_tests();
    std::cout << "Test suite: OK" << std::endl;
}
