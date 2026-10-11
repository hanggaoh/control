#include "rule_of_zero_buffer.hpp"

#include <cstdlib>
#include <iostream>
#include <utility>

using interview::foundations::practice::rule_of_zero_buffer;

int main() {
    int failures = 0;
    auto check = [&](bool condition, const char* message) {
        if (!condition) {
            ++failures;
            std::cerr << message << '\n';
        }
    };

    rule_of_zero_buffer source{2};
    check(source[0] == 0 && source[1] == 0, "initial values");
    source[0] = 10;
    source[1] = 20;

    auto copied = source;
    copied[0] = 99;
    check(source[0] == 10 && copied[0] == 99, "independent copy");

    rule_of_zero_buffer target{1};
    target[0] = 7;
    target = source;
    source[1] = 30;
    check(target.size() == 2 && target[0] == 10 && target[1] == 20,
          "assignment replaces old storage with independent contents");
    target = target;
    check(target.size() == 2 && target[1] == 20, "self-assignment");

    rule_of_zero_buffer moved{std::move(target)};
    check(moved.size() == 2 && moved[1] == 20, "move construction");
    target = rule_of_zero_buffer{1}; // reuse, without assuming moved-from size
    target[0] = 42;
    check(target[0] == 42, "moved-from object can be reassigned");

    rule_of_zero_buffer empty{0};
    moved = empty;
    check(moved.size() == 0, "assignment from empty");
    empty = source;
    check(empty.size() == 2 && empty[1] == 30, "assignment to empty");

    using std::swap;
    swap(moved, empty);
    const auto& observed = moved;
    check(observed.size() == 2 && observed[0] == 10 && empty.size() == 0,
          "generic swap and const access");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
