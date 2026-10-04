#pragma once

namespace interview::control::filter_practice {

inline constexpr bool exercise_complete = false; // TODO: set true when complete.

class low_pass_filter {
public:
    // Contract: alpha is validated by the caller and lies in [0, 1].
    explicit low_pass_filter(double alpha) noexcept : alpha_(alpha) {}

    [[nodiscard]] double update(double input) noexcept {
        // TODO: initialize from the first sample, then apply the recurrence.
        (void)input;
        return output_;
    }

    void reset() noexcept {
        output_ = 0.0;
        initialized_ = false;
    }

private:
    double alpha_{};
    double output_{};
    bool initialized_{};
};

}  // namespace interview::control::filter_practice
