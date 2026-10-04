#pragma once

namespace interview::database::transaction_practice {

inline constexpr bool exercise_complete = false; // TODO: set true when complete.

class fake_connection {
public:
    void begin() noexcept { ++begins; }
    void commit() noexcept { ++commits; }
    void rollback() noexcept { ++rollbacks; }

    int begins{};
    int commits{};
    int rollbacks{};
};

class transaction_guard {
public:
    explicit transaction_guard(fake_connection& connection) noexcept
        : connection_(&connection), active_(true) {
        connection_->begin();
    }

    transaction_guard(const transaction_guard&) = delete;
    transaction_guard& operator=(const transaction_guard&) = delete;

    // TODO: transfer the active transaction and make other inactive.
    transaction_guard(transaction_guard&& other) noexcept
        : connection_(nullptr), active_(false) {
        (void)other;
    }

    transaction_guard& operator=(transaction_guard&&) = delete;

    // TODO: rollback exactly once if the transaction is still active.
    ~transaction_guard() noexcept = default;

    // TODO: commit exactly once and make the guard inactive.
    void commit() noexcept {}

    [[nodiscard]] bool active() const noexcept { return active_; }

private:
    fake_connection* connection_{};
    bool active_{};
};

}  // namespace interview::database::transaction_practice
