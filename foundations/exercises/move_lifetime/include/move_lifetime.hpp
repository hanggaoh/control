#pragma once

#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace interview::foundations::move_lifetime_practice {

inline constexpr bool exercise_complete = false; // TODO: set true after completing the exercise.

struct relocation_probe {
    explicit relocation_probe(int value) : value(value) {}

    // TODO: preserve value and increment copies.
    relocation_probe(const relocation_probe& other) : value(other.value) {
        // TODO: increment copies.
    }

    // TODO: preserve value, increment moves, and leave other in a valid state.
    relocation_probe(relocation_probe&& other) noexcept : value(other.value) {
        // TODO: increment moves and leave other in a documented valid state.
    }

    int value{};
    static inline int copies{};
    static inline int moves{};
};

struct throwing_move_probe {
    explicit throwing_move_probe(int value) : value(value) {}

    throwing_move_probe(const throwing_move_probe& other) : value(other.value) {
        // TODO: increment copies.
    }

    throwing_move_probe(throwing_move_probe&& other) noexcept(false) : value(other.value) {
        // TODO: increment moves. Do not actually throw in this observation exercise.
    }

    int value{};
    static inline int copies{};
    static inline int moves{};
};

template <typename T>
[[nodiscard]] constexpr decltype(auto) relocation_source(T& value) noexcept {
    // TODO: use the standard utility that selects move only when it is safe
    // for the strong guarantee (or copying is unavailable).
    return std::move(value);
}

class named_owner {
public:
    explicit named_owner(std::string name) : name_(std::move(name)) {}

    // TODO: return a non-owning view into name_. State its invalidation contract.
    [[nodiscard]] std::string_view stable_name_view() const noexcept { return {}; }

private:
    std::string name_;
};

}  // namespace interview::foundations::move_lifetime_practice
