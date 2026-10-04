#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>

namespace interview::foundations::shared_ownership_practice {

inline constexpr bool exercise_complete = false; // TODO: set true after completing the exercise.

struct device {
    explicit device(std::string name) : name(std::move(name)) {}
    std::string name;
};

// TODO: return another shared owner using the existing control block.
// Never construct a new shared_ptr from existing.get().
[[nodiscard]] inline std::shared_ptr<device> share_existing(
    const std::shared_ptr<device>& existing) {
    (void)existing;
    return {};
}

class async_session : public std::enable_shared_from_this<async_session> {
public:
    [[nodiscard]] static std::shared_ptr<async_session> create(std::string name) {
        return std::shared_ptr<async_session>(new async_session(std::move(name)));
    }

    // TODO: return a callback that safely keeps this session alive until the
    // callback object is destroyed. Capture shared_from_this(), never
    // std::shared_ptr<async_session>(this) and never a bare this pointer.
    [[nodiscard]] std::function<std::string()> make_callback() {
        return {};
    }

private:
    explicit async_session(std::string name) : name_(std::move(name)) {}
    std::string name_;
};

[[nodiscard]] inline bool same_owner(const std::shared_ptr<device>& lhs,
                                     const std::shared_ptr<device>& rhs) noexcept {
    return !lhs.owner_before(rhs) && !rhs.owner_before(lhs);
}

}  // namespace interview::foundations::shared_ownership_practice
