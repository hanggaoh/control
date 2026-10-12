#pragma once

#include <concepts>
#include <string>
#include <type_traits>
#include <utility>

namespace interview::foundations::syntax_practice {

inline constexpr bool deduction_complete = true;
inline constexpr bool reference_complete = true;
inline constexpr bool functions_complete = true;
inline constexpr bool classes_complete = true;
inline constexpr bool categories_complete = true;
inline constexpr bool polymorphism_complete = true;
inline constexpr bool templates_complete = true;

inline constexpr int value = 3;
using borrowed_type = decltype((value));

inline void increment(int& target) noexcept {
    ++target;
}

struct reading {
    int value{};
    int& get() noexcept { return value; }
    const int& get() const noexcept { return value; }
};

class device_name {
public:
    explicit device_name(std::string name) : name_(std::move(name)) {}
    const std::string& get() const noexcept { return name_; }
private:
    std::string name_;
};

inline char inspect(const std::string&) noexcept { return 'A'; }
inline char inspect(std::string&&) noexcept { return 'B'; }

template <typename T>
char forward_inspect(T&& argument) noexcept {
    return inspect(std::forward<T>(argument));
}

struct sensor {
    virtual ~sensor() = default;
    virtual int read() const noexcept { return 0; }
};
struct simulated_sensor : sensor {
    int read() const noexcept override {
        return 42;
    }
};

template <std::integral T>
constexpr T twice(T input) noexcept {
    return input * 2;
}

}  // namespace interview::foundations::syntax_practice
