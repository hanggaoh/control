#include "pid_controller.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace interview::control::pid_practice;

namespace {
bool near(double lhs, double rhs) { return std::abs(lhs - rhs) < 1e-9; }
}

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): PID controller exercise\n";
        return EXIT_SUCCESS;
    }

    pid_controller proportional{{.kp = 2.0, .ki = 0.0, .kd = 0.0,
                                 .sample_time = 0.1, .output_min = -10.0,
                                 .output_max = 10.0}};
    if (!near(proportional.update(3.0, 1.0), 4.0)) {
        return EXIT_FAILURE;
    }

    pid_controller limited{{.kp = 0.0, .ki = 1.0, .kd = 0.0,
                            .sample_time = 1.0, .output_min = -2.0,
                            .output_max = 2.0}};
    if (!near(limited.update(10.0, 0.0), 2.0) ||
        !near(limited.integral_state(), 0.0)) {
        return EXIT_FAILURE;
    }

    pid_controller derivative{{.kp = 0.0, .ki = 0.0, .kd = 1.0,
                               .sample_time = 0.5, .output_min = -100.0,
                               .output_max = 100.0}};
    if (!near(derivative.update(4.0, 1.0), 0.0) ||
        !near(derivative.update(4.0, 2.0), -2.0)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
