#pragma once

#include <concepts>
#include <string>
#include <type_traits>
#include <utility>

namespace interview::foundations::syntax_practice {

inline constexpr bool deduction_complete = false;
inline constexpr bool reference_complete = false;
inline constexpr bool functions_complete = false;
inline constexpr bool classes_complete = false;
inline constexpr bool categories_complete = false;
inline constexpr bool polymorphism_complete = false;
inline constexpr bool templates_complete = false;

// TODO: choose decltype(value) or decltype((value)) to produce a const int&.
inline constexpr int value = 3;
using borrowed_type = int;

inline void increment(int& target) noexcept {
    // TODO: change the caller's value by one.
    (void)target;
}

struct reading {
    int value{};
    // TODO: mutable object returns int&; const object returns const int&.
    int get() noexcept { return value; }
    int get() const noexcept { return value; }
};

class device_name {
public:
    // TODO: require explicit construction; initialize name_ from the argument.
    device_name(std::string name) { (void)name; }
    const std::string& get() const noexcept { return name_; }
private:
    std::string name_;
};

inline char inspect(const std::string&) noexcept { return 'A'; }
inline char inspect(std::string&&) noexcept { return 'B'; }

template <typename T>
char forward_inspect(T&& argument) noexcept {
    // TODO: preserve the argument's category when calling inspect.
    return inspect(argument);
}

struct sensor {
    virtual ~sensor() = default;
    virtual int read() const noexcept { return 0; }
};
struct simulated_sensor : sensor {
    // TODO: override read() to return 42 with the same const/noexcept contract.
};

template <std::integral T>
constexpr T twice(T input) noexcept {
    // TODO: return twice the input; tests use small representable values.
    return input;
}

}  // namespace interview::foundations::syntax_practice
