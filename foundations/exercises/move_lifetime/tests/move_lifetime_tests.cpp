#include "move_lifetime.hpp"

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <vector>

using namespace interview::foundations::move_lifetime_practice;

static_assert(std::is_nothrow_move_constructible_v<relocation_probe>);
static_assert(!std::is_nothrow_move_constructible_v<throwing_move_probe>);

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): noexcept move/lifetime exercise\n";
        return EXIT_SUCCESS;
    }

    relocation_probe::copies = 0;
    relocation_probe::moves = 0;
    std::vector<relocation_probe> values;
    values.reserve(1);
    values.emplace_back(42);
    values.emplace_back(7); // forces relocation from capacity 1

    throwing_move_probe::copies = 0;
    throwing_move_probe::moves = 0;
    throwing_move_probe source{9};
    throwing_move_probe relocated{relocation_source(source)};

    named_owner owner{"pump-01"};
    const bool ok = values[0].value == 42 && relocation_probe::moves >= 1 &&
                    relocation_probe::copies == 0 && relocated.value == 9 &&
                    throwing_move_probe::copies == 1 && throwing_move_probe::moves == 0 &&
                    owner.stable_name_view() == "pump-01";
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
