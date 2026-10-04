#pragma once

#include <algorithm>

namespace interview::control::pid_practice {

inline constexpr bool exercise_complete = false; // TODO: set true when complete.

struct pid_config {
    double kp{};
    double ki{};
    double kd{};
    double sample_time{1.0};
    double output_min{-1.0};
    double output_max{1.0};
};

class pid_controller {
public:
    explicit pid_controller(pid_config config) noexcept : config_(config) {}

    [[nodiscard]] double update(double setpoint, double measurement) noexcept {
        // TODO: implement parallel-form PID with output clamping and
        // conditional-integration anti-windup. On the first sample, use a zero
        // derivative term. Assume config has already been validated.
        (void)setpoint;
        (void)measurement;
        return 0.0;
    }

    void reset() noexcept {
        integral_ = 0.0;
        previous_error_ = 0.0;
        initialized_ = false;
    }

    [[nodiscard]] double integral_state() const noexcept { return integral_; }

private:
    pid_config config_;
    double integral_{};
    double previous_error_{};
    bool initialized_{};
};

}  // namespace interview::control::pid_practice
