#ifndef test_main_h
#define test_main_h

#include <algorithm>
#include <functional>
#include <vector>

using tests_t = std::vector<std::function<void()>>;

tests_t& get_tests() {
    static tests_t tests;
    return tests;
}

template <typename F>
bool add_test(F f) {
    get_tests().push_back(f);
    return true;
}

void run_tests() {
    const auto& tests = get_tests();
    std::for_each(std::begin(tests), std::end(tests), [](const auto& f) { f(); });
}

#endif
