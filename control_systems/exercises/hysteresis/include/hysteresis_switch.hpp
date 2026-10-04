#pragma once

namespace interview::control::hysteresis_practice {

inline constexpr bool exercise_complete = false; // TODO: set true when complete.

class hysteresis_switch {
public:
    hysteresis_switch(double low, double high, bool initial = false) noexcept
        : low_(low), high_(high), state_(initial) {}

    [[nodiscard]] bool update(double input) noexcept {
        // TODO: use different transition thresholds for OFF->ON and ON->OFF.
        (void)input;
        return state_;
    }

    [[nodiscard]] bool state() const noexcept { return state_; }

private:
    double low_{};
    double high_{};
    bool state_{};
};

}  // namespace interview::control::hysteresis_practice
