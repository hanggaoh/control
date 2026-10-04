#pragma once

#include <concepts>
#include <utility>

namespace interview {

template <typename Traits, typename Handle>
concept handle_traits_for = requires(Handle handle) {
    { Traits::invalid() } noexcept -> std::same_as<Handle>;
    { Traits::close(handle) } noexcept -> std::same_as<void>;
};

// Owns at most one native resource handle.
//
// Traits must provide:
//   static Handle invalid() noexcept;
//   static void close(Handle) noexcept;
template <typename Handle, typename Traits>
    requires handle_traits_for<Traits, Handle>
class unique_handle {
public:
    using handle_type = Handle;

    constexpr unique_handle() noexcept = default;
    explicit constexpr unique_handle(handle_type handle) noexcept : handle_(handle) {}

    unique_handle(const unique_handle&) = delete;
    unique_handle& operator=(const unique_handle&) = delete;

    // TODO: Transfer ownership and leave other empty.
    constexpr unique_handle(unique_handle&& other) noexcept
        : handle_(other.handle_) {}

    // TODO: Release the currently owned handle, then transfer ownership from other.
    constexpr unique_handle& operator=(unique_handle&& other) noexcept {
        handle_ = other.handle_;
        return *this;
    }

    // TODO: Close the owned handle exactly once, if it is valid.
    constexpr ~unique_handle() noexcept = default;

    [[nodiscard]] constexpr handle_type get() const noexcept { return handle_; }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return handle_ != Traits::invalid();
    }

    // TODO: Return the owned handle and leave *this empty without closing it.
    [[nodiscard]] constexpr handle_type release() noexcept {
        return Traits::invalid();
    }

    // TODO: Close the old handle exactly once, then take ownership of replacement.
    // Be careful when replacement is already the owned handle.
    constexpr void reset(handle_type replacement = Traits::invalid()) noexcept {
        handle_ = replacement;
    }

    constexpr void swap(unique_handle& other) noexcept {
        using std::swap;
        swap(handle_, other.handle_);
    }

private:
    handle_type handle_{Traits::invalid()};
};

template <typename Handle, typename Traits>
constexpr void swap(unique_handle<Handle, Traits>& lhs,
                    unique_handle<Handle, Traits>& rhs) noexcept {
    lhs.swap(rhs);
}

}  // namespace interview
