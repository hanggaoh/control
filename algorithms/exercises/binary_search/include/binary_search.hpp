#pragma once

#include <cstddef>
#include <span>

namespace interview::algorithms::binary_search_practice {

inline constexpr bool exercise_complete = false; // TODO: set true when complete.

[[nodiscard]] constexpr std::size_t first_not_less(std::span<const int> values,
                                                    int target) noexcept {
    // TODO: implement lower-bound semantics over [first, last).
    (void)target;
    return values.size();
}

}  // namespace interview::algorithms::binary_search_practice
