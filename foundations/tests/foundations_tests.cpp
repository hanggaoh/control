#include "cpp_foundations.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string_view>
#include <type_traits>

namespace interview::foundations {

// This definition supplies storage/code for the declaration in the header.
int add(int lhs, int rhs) noexcept { return lhs + rhs; }

}  // namespace interview::foundations

namespace {

int failures = 0;

void check(bool condition, std::string_view expression, int line) {
    if (!condition) {
        ++failures;
        std::cerr << "line " << line << ": CHECK(" << expression << ") failed\n";
    }
}

#define CHECK(expression) check(static_cast<bool>(expression), #expression, __LINE__)

static_assert(std::same_as<decltype(interview::foundations::module_name),
                           const std::string_view>);
static_assert(interview::foundations::clamp_value(12, 0, 10) == 10);
static_assert(!std::is_copy_constructible_v<std::unique_ptr<int>>);

}  // namespace

int main() {
    using namespace interview::foundations;

    CHECK(add(2, 3) == 5);
    CHECK(clamp_value(7, 0, 10) == 7);

    sample_buffer buffer{3};
    buffer.values()[1] = 42;
    CHECK(buffer.values().size() == 3);
    CHECK(buffer.values()[1] == 42);

    CHECK(!parse_non_negative(-1).has_value());
    CHECK(parse_non_negative(9) == 9);

    auto name = make_device_name("pump-01");
    CHECK(name->view() == "pump-01");

    if (failures != 0) {
        std::cerr << failures << " foundation check(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << module_name << " checks passed\n";
    return EXIT_SUCCESS;
}
