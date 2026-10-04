#include "binary_search.hpp"

#include <array>
#include <cstdlib>
#include <iostream>

using namespace interview::algorithms::binary_search_practice;

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): binary-search exercise\n";
        return EXIT_SUCCESS;
    }

    constexpr std::array values{1, 3, 3, 7, 10};
    if (first_not_less(values, 3) != 1 || first_not_less(values, 4) != 3 ||
        first_not_less(values, -1) != 0 || first_not_less(values, 11) != values.size()) {
        return EXIT_FAILURE;
    }

    constexpr std::array<int, 0> empty{};
    return first_not_less(empty, 1) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
