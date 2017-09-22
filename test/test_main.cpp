#include "test_main.hpp"

tests_t& get_tests() {
    static tests_t tests;
    return tests;
}

void run_tests() {
    const auto& tests = get_tests();
    std::for_each(std::begin(tests), std::end(tests), [](const auto& f) { f(); });
}
