/**
 * This file defines a very simple unit tests framework.
 */

#ifndef test_main_h
#define test_main_h

#include <algorithm>
#include <functional>
#include <vector>

/**
 * The container of tests is a simple vector
 * of functors.
 */
using tests_t = std::vector<std::function<void()>>;

/**
 * The static vector of tests is shared among
 * different files. This function provides this
 * unique instance.
 * @return - the vector of unit tests.
 */
tests_t& get_tests() {
    static tests_t tests;
    return tests;
}

/**
 * Add a new unit test to the unique vector of
 * tests.
 * @param f - function, function object or lamdba
 *            expression.
 * &return - always true so that the result of this
 *           function may be assigned to a static
 *           variable in order to force the population
 *           of the vector of unit tests at startup
 *           time.
 */
template <typename F>
bool add_test(F f) {
    get_tests().push_back(f);
    return true;
}

/**
 * Run all the unit tests captured in the single instance
 * of the vector of tests.
 */
void run_tests() {
    const auto& tests = get_tests();
    std::for_each(std::begin(tests), std::end(tests), [](const auto& f) { f(); });
}

#endif
