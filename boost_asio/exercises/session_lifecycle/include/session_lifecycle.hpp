#pragma once

#include <cstdint>

namespace interview::asio::lifecycle_practice {

inline constexpr bool exercise_complete = false; // TODO: set true when complete.

enum class session_state { stopped, connecting, connected, backoff, stopping };

class session_lifecycle {
public:
    [[nodiscard]] std::uint64_t start() noexcept {
        // TODO: begin a new connecting generation unless already active.
        return generation_;
    }

    void on_connected(std::uint64_t generation) noexcept {
        // TODO: accept only a current connecting completion.
        (void)generation;
    }

    void on_failure(std::uint64_t generation) noexcept {
        // TODO: current active failures enter backoff; stale/stop completions do nothing.
        (void)generation;
    }

    [[nodiscard]] std::uint64_t reconnect() noexcept {
        // TODO: only backoff may begin a new connecting generation.
        return generation_;
    }

    void stop() noexcept {
        // TODO: invalidate outstanding completions and enter stopping.
    }

    void complete_stop() noexcept {
        // TODO: stopping becomes stopped.
    }

    [[nodiscard]] session_state state() const noexcept { return state_; }
    [[nodiscard]] std::uint64_t generation() const noexcept { return generation_; }

private:
    session_state state_{session_state::stopped};
    std::uint64_t generation_{};
};

}  // namespace interview::asio::lifecycle_practice
