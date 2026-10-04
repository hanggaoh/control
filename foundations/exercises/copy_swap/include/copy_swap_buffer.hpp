#pragma once

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace interview::foundations::practice {

inline constexpr bool exercise_complete = false; // TODO: set true after completing the exercise.

class copy_swap_buffer {
public:
    explicit copy_swap_buffer(std::size_t size)
        : size_(size), data_(size == 0 ? nullptr : new int[size]{}) {}

    // TODO: release the owned array.
    ~copy_swap_buffer() { delete[] data_; }

    // TODO: deep-copy other. Call maybe_throw_copy() before allocation.
    copy_swap_buffer(const copy_swap_buffer& other)
        : size_(0), data_(nullptr) {
        (void)other;
    }

    // TODO: implement copy-and-swap. Do not add a self-assignment special case.
    copy_swap_buffer& operator=(const copy_swap_buffer& other) {
        (void)other;
        return *this;
    }

    // TODO: exchange every representation member; this operation must not throw.
    void swap(copy_swap_buffer& other) noexcept { (void)other; }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    int& operator[](std::size_t index) noexcept { return data_[index]; }
    const int& operator[](std::size_t index) const noexcept { return data_[index]; }

    static void fail_next_copy(bool value) noexcept { fail_copy_ = value; }

private:
    static void maybe_throw_copy() {
        if (fail_copy_) {
            fail_copy_ = false;
            throw std::runtime_error("injected copy failure");
        }
    }

    std::size_t size_{};
    int* data_{};
    static inline bool fail_copy_{};
};

// TODO: delegate to the member swap and preserve noexcept.
inline void swap(copy_swap_buffer& lhs, copy_swap_buffer& rhs) noexcept {
    (void)lhs;
    (void)rhs;
}

}  // namespace interview::foundations::practice
