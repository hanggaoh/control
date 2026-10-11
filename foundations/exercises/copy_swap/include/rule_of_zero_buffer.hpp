#pragma once

#include <cstddef>
#include <vector>

namespace interview::foundations::practice {

// Rule of Zero: vector owns storage and supplies copy/move/destruction.
class rule_of_zero_buffer {
public:
    explicit rule_of_zero_buffer(std::size_t size) : data_(size) {}

    [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }
    // Precondition: index < size().
    int& operator[](std::size_t index) noexcept { return data_[index]; }
    const int& operator[](std::size_t index) const noexcept { return data_[index]; }

private:
    std::vector<int> data_;
};

}  // namespace interview::foundations::practice
