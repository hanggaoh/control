#pragma once

#include <algorithm>
#include <concepts>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace interview::foundations {

// An inline variable may be defined in a header included by many translation units.
inline constexpr std::string_view module_name = "C++ Foundations";

// A declaration introduces a name and type. The definition is in the test .cpp.
[[nodiscard]] int add(int lhs, int rhs) noexcept;

// Templates normally keep their definition visible in the header because the
// compiler needs it when a concrete specialization is instantiated.
template <std::totally_ordered T>
[[nodiscard]] constexpr T clamp_value(T value, const T& low, const T& high) {
    return std::clamp(std::move(value), low, high);
}

class sample_buffer {
public:
    // Member initialization happens before the constructor body runs.
    explicit sample_buffer(std::size_t size) : values_(size, 0) {}

    [[nodiscard]] std::span<int> values() noexcept { return values_; }
    [[nodiscard]] std::span<const int> values() const noexcept { return values_; }

private:
    // Value member: lifetime follows sample_buffer automatically.
    std::vector<int> values_;
};

class device_name {
public:
    explicit device_name(std::string value) : value_(std::move(value)) {}

    [[nodiscard]] std::string_view view() const noexcept { return value_; }

private:
    std::string value_;
};

[[nodiscard]] inline std::optional<int> parse_non_negative(int value) noexcept {
    if (value < 0) {
        return std::nullopt;
    }
    return value;
}

[[nodiscard]] inline std::unique_ptr<device_name> make_device_name(std::string value) {
    return std::make_unique<device_name>(std::move(value));
}

}  // namespace interview::foundations
