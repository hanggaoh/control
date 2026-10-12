#include "migration_32_to_64.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>

using namespace interview::foundations::migration_practice;

int main() {
    int failures = 0;
    const auto report = [&](bool enabled, bool passed, const char* group) {
        std::cout << (enabled ? (passed ? "PASS " : "FAIL ") : "SKIPPED ") << group << '\n';
        if (enabled && !passed) {
            ++failures;
        }
    };

    int object = 7;
    const auto encoded = address_value(&object);
    report(pointer_width_complete,
           reinterpret_cast<const void*>(encoded) == &object,
           "pointer width");

    constexpr auto maximum = std::numeric_limits<std::uint32_t>::max();
    const auto small = protocol_length(42);
    const auto largest = protocol_length(static_cast<std::size_t>(maximum));
    bool boundary_ok = small == 42 && largest == maximum;
    if constexpr (std::numeric_limits<std::size_t>::max() > maximum) {
        boundary_ok = boundary_ok &&
            !protocol_length(static_cast<std::size_t>(maximum) + 1).has_value();
    }
    report(boundary_conversion_complete, boundary_ok, "boundary conversion");

    report(wire_format_complete,
           sizeof(wire_header::payload_length) == 4 &&
               sizeof(wire_header::sequence_number) == 8 &&
               std::is_unsigned_v<decltype(wire_header::payload_length)> &&
               std::is_unsigned_v<decltype(wire_header::sequence_number)>,
           "wire field widths");

    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

