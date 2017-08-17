#ifndef algorithms_test_h
#define algorithms_test_h

#include "test_main.hpp"
#include "algorithms.hpp"

#include <array>
#include <vector>

struct point2d {
    int x{};
    int y{};
};

bool operator==(const point2d& p1, const point2d& p2) {
    return hull::equals(p1, p2);
}

#include "graham_scan_test.hpp"
#include "monotone_chain_test.hpp"

#endif
