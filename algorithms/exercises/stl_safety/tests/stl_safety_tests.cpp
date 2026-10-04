#include "stl_safety.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace interview::algorithms::stl_safety_practice;

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): STL safety exercise\n";
        return EXIT_SUCCESS;
    }

    std::vector<int> mixed{3, -1, -2, 4, 0, -5, 8};
    erase_negative(mixed);
    if (mixed != std::vector<int>{3, 4, 0, 8}) {
        return EXIT_FAILURE;
    }

    std::vector<int> all_negative{-3, -2, -1};
    erase_negative(all_negative);
    if (!all_negative.empty()) {
        return EXIT_FAILURE;
    }

    std::vector<int> empty;
    erase_negative(empty);
    return empty.empty() ? EXIT_SUCCESS : EXIT_FAILURE;
}
