/**
 * The main function simply runs all the unit tests.
 * If a unit test fails, it stops immediately the process.
 * If all unit tests pass, a success message is printed
 * to the standard output.
 */

#include "test/test_main.hpp"
#include "test/point_concept_test.hpp"
#include "test/angle_test.hpp"
#include "test/graham_scan_test.hpp"
#include "test/monotone_chain_test.hpp"
#include "test/jarvis_march_test.hpp"
#include "test/chan_test.hpp"
#include "test/algorithms_test.hpp"

#include <iostream>

int main() {
    run_tests();
    std::cout << "Test suite: OK" << std::endl;
}
