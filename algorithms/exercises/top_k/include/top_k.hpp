#pragma once

#include <cstddef>
#include <span>
#include <vector>

namespace interview::algorithms::top_k_practice {

inline constexpr bool exercise_complete = false; // TODO: set true when complete.

[[nodiscard]] inline std::vector<int> top_k_largest(std::span<const int> values,
                                                     std::size_t k) {
    // TODO: maintain a bounded min-heap, then return results in descending order.
    (void)values;
    (void)k;
    return {};
}

}  // namespace interview::algorithms::top_k_practice
