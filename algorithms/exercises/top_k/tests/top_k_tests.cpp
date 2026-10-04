#include "top_k.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace interview::algorithms::top_k_practice;

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): top-K exercise\n";
        return EXIT_SUCCESS;
    }

    const std::vector<int> values{5, 1, 9, 3, 9, -2, 7};
    if (top_k_largest(values, 3) != std::vector<int>{9, 9, 7}) {
        return EXIT_FAILURE;
    }
    if (top_k_largest(values, 0) != std::vector<int>{}) {
        return EXIT_FAILURE;
    }
    if (top_k_largest(std::vector<int>{2, 1}, 5) != std::vector<int>{2, 1}) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
