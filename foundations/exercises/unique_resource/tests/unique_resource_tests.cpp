#include "unique_resource.hpp"

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <utility>

using namespace interview::foundations::unique_resource_practice;

static_assert(!std::is_copy_constructible_v<tracked_resource>);
static_assert(std::is_nothrow_move_assignable_v<tracked_resource>);

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): self-move unique resource exercise\n";
        return EXIT_SUCCESS;
    }

    tracked_resource::reset_counts();
    {
        tracked_resource value{42};
        // Go through an alias to model self-move arriving via generic code and
        // avoid a compiler warning that would reveal the test mechanically.
        tracked_resource* alias = &value;
        value = std::move(*alias);
        if (!value.valid() || value.get() != 42 || tracked_resource::releases() != 0) {
            return EXIT_FAILURE;
        }

        tracked_resource destination{7};
        destination = std::move(value);
        if (destination.get() != 42 || value.valid() || tracked_resource::releases() != 1) {
            return EXIT_FAILURE;
        }
    }

    return tracked_resource::releases() == 2 ? EXIT_SUCCESS : EXIT_FAILURE;
}
