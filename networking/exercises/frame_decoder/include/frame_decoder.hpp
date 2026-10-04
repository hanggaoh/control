#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace interview::networking::frame_practice {

inline constexpr bool exercise_complete = false; // TODO: set true when complete.

class frame_decoder {
public:
    explicit frame_decoder(std::size_t max_payload) : max_payload_(max_payload) {}

    [[nodiscard]] std::vector<std::vector<std::byte>> feed(std::span<const std::byte> bytes) {
        // TODO: append bytes and extract every complete length-prefixed frame.
        // Validate the big-endian uint32 length before constructing a payload.
        (void)bytes;
        return {};
    }

    [[nodiscard]] bool failed() const noexcept { return failed_; }
    [[nodiscard]] std::size_t buffered_bytes() const noexcept { return buffer_.size(); }

private:
    std::size_t max_payload_{};
    std::vector<std::byte> buffer_;
    bool failed_{};
};

}  // namespace interview::networking::frame_practice
