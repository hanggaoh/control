#pragma once

#include <utility>

namespace interview::foundations::unique_resource_practice {

inline constexpr bool exercise_complete = false; // TODO: set true after completing the exercise.

class tracked_resource {
public:
    explicit tracked_resource(int id = -1) noexcept : id_(id) {}
    tracked_resource(const tracked_resource&) = delete;
    tracked_resource& operator=(const tracked_resource&) = delete;

    tracked_resource(tracked_resource&& other) noexcept
        : id_(std::exchange(other.id_, -1)) {}

    // TODO: implement move assignment so ownership transfers exactly once and
    // x = std::move(x) preserves x's resource. Keep the operation noexcept.
    tracked_resource& operator=(tracked_resource&& other) noexcept {
        (void)other;
        return *this;
    }

    ~tracked_resource() noexcept { release(); }

    [[nodiscard]] int get() const noexcept { return id_; }
    [[nodiscard]] bool valid() const noexcept { return id_ != -1; }

    static void reset_counts() noexcept { releases_ = 0; }
    [[nodiscard]] static int releases() noexcept { return releases_; }

private:
    void release() noexcept {
        if (valid()) {
            ++releases_;
            id_ = -1;
        }
    }

    int id_{-1};
    static inline int releases_{};
};

}  // namespace interview::foundations::unique_resource_practice
