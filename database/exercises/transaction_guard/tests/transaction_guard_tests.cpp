#include "transaction_guard.hpp"

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <utility>

using namespace interview::database::transaction_practice;

static_assert(!std::is_copy_constructible_v<transaction_guard>);
static_assert(std::is_nothrow_move_constructible_v<transaction_guard>);

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): transaction guard exercise\n";
        return EXIT_SUCCESS;
    }

    fake_connection committed;
    {
        transaction_guard transaction{committed};
        transaction.commit();
        transaction.commit();
    }
    if (committed.begins != 1 || committed.commits != 1 || committed.rollbacks != 0) {
        return EXIT_FAILURE;
    }

    fake_connection rolled_back;
    {
        transaction_guard transaction{rolled_back};
    }
    if (rolled_back.begins != 1 || rolled_back.commits != 0 || rolled_back.rollbacks != 1) {
        return EXIT_FAILURE;
    }

    fake_connection moved;
    {
        transaction_guard first{moved};
        transaction_guard second{std::move(first)};
        if (first.active() || !second.active()) {
            return EXIT_FAILURE;
        }
    }
    return moved.rollbacks == 1 ? EXIT_SUCCESS : EXIT_FAILURE;
}
