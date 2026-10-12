#pragma once

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <memory>

namespace interview::foundations::practice {

inline constexpr bool exercise_complete = true;

class copy_swap_buffer {
public:
    explicit copy_swap_buffer(std::size_t size)
        : size_(size), data_(size == 0 ? nullptr : new int[size]{}) {}

    ~copy_swap_buffer() { delete[] data_; }

    copy_swap_buffer(const copy_swap_buffer& other)
        : size_(0), data_(nullptr) {
        auto candidate = other.size_ == 0
            ? std::unique_ptr<int[]>{}
            : std::make_unique<int[]>(other.size_);

        maybe_throw_copy();

        if (other.size_ != 0) {
            std::copy_n(other.data_, other.size_, candidate.get());
       }

        size_ = other.size_;
        data_ = candidate.release();
    }

    copy_swap_buffer& operator=(const copy_swap_buffer& other) {
        copy_swap_buffer temporary(other);
        swap(temporary);
        return *this;
    }

    void swap(copy_swap_buffer& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

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

inline void swap(copy_swap_buffer& lhs, copy_swap_buffer& rhs) noexcept {
    lhs.swap(rhs);
}

}  // namespace interview::foundations::practice
