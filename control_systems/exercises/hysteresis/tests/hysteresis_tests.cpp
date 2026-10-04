#include "hysteresis_switch.hpp"

#include <cstdlib>
#include <iostream>

using namespace interview::control::hysteresis_practice;

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): hysteresis exercise\n";
        return EXIT_SUCCESS;
    }

    hysteresis_switch control{40.0, 60.0};
    if (control.update(50.0) || !control.update(60.0) ||
        !control.update(50.0) || !control.update(40.1) ||
        control.update(40.0)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
