#include "copy_swap_buffer.hpp"

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <utility>

using interview::foundations::practice::copy_swap_buffer;

static_assert(std::is_nothrow_swappable_v<copy_swap_buffer>);

int main() {
    if (!interview::foundations::practice::exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): copy-and-swap exercise\n";
        return EXIT_SUCCESS;
    }

    int failures = 0;
    auto check = [&](bool condition) { failures += condition ? 0 : 1; };

    copy_swap_buffer source{2};
    source[0] = 10;
    source[1] = 20;
    copy_swap_buffer copy{source};
    source[0] = 99;
    check(copy.size() == 2 && copy[0] == 10 && copy[1] == 20);

    copy = copy;
    check(copy.size() == 2 && copy[0] == 10 && copy[1] == 20);

    copy_swap_buffer target{1};
    target[0] = 7;
    copy_swap_buffer::fail_next_copy(true);
    try {
        target = source;
        check(false);
    } catch (const std::runtime_error&) {
        check(target.size() == 1 && target[0] == 7);
    }

    copy_swap_buffer left{1};
    copy_swap_buffer right{2};
    using std::swap;
    swap(left, right);
    check(left.size() == 2 && right.size() == 1);

    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
