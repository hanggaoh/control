#include "session_lifecycle.hpp"

#include <cstdlib>
#include <iostream>

using namespace interview::asio::lifecycle_practice;

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): Asio lifecycle exercise\n";
        return EXIT_SUCCESS;
    }

    session_lifecycle lifecycle;
    const auto first = lifecycle.start();
    lifecycle.on_failure(first);
    const auto second = lifecycle.reconnect();
    lifecycle.on_connected(first); // stale completion must be ignored
    if (lifecycle.state() != session_state::connecting || first == second) return EXIT_FAILURE;
    lifecycle.on_connected(second);
    if (lifecycle.state() != session_state::connected) return EXIT_FAILURE;
    lifecycle.stop();
    lifecycle.on_failure(second); // cancelled old operation must not schedule backoff
    if (lifecycle.state() != session_state::stopping) return EXIT_FAILURE;
    lifecycle.complete_stop();
    return lifecycle.state() == session_state::stopped ? EXIT_SUCCESS : EXIT_FAILURE;
}
