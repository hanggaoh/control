#include "low_pass_filter.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace interview::control::filter_practice;

namespace {
bool near(double lhs, double rhs) { return std::abs(lhs - rhs) < 1e-9; }
}

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): low-pass filter exercise\n";
        return EXIT_SUCCESS;
    }

    low_pass_filter filter{0.25};
    if (!near(filter.update(8.0), 8.0) || !near(filter.update(12.0), 9.0) ||
        !near(filter.update(12.0), 9.75)) {
        return EXIT_FAILURE;
    }

    filter.reset();
    return near(filter.update(-4.0), -4.0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
