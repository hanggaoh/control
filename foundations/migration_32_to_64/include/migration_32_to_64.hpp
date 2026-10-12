#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>

namespace interview::foundations::migration_practice {

inline constexpr bool pointer_width_complete = false;
inline constexpr bool boundary_conversion_complete = false;
inline constexpr bool wire_format_complete = false;

// TODO: return an integer representation that cannot truncate a valid pointer.
[[nodiscard]] inline std::uintptr_t address_value(const void* pointer) noexcept {
    (void)pointer;
    return 0;
}

// TODO: convert a process-local size to the protocol's 32-bit length only when
// the value is representable. Return std::nullopt otherwise.
[[nodiscard]] constexpr std::optional<std::uint32_t>
protocol_length(std::size_t size) noexcept {
    (void)size;
    return std::nullopt;
}

struct wire_header {
    // TODO: choose fixed-width unsigned types: 32 bits for payload length and
    // 64 bits for sequence number. Do not rely on long or std::size_t.
    unsigned long payload_length{};
    unsigned long sequence_number{};
};

}  // namespace interview::foundations::migration_practice
