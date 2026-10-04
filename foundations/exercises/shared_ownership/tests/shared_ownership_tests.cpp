#include "shared_ownership.hpp"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

using namespace interview::foundations::shared_ownership_practice;

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): shared ownership exercise\n";
        return EXIT_SUCCESS;
    }

    auto first = std::make_shared<device>("sensor-01");
    auto second = share_existing(first);
    if (second.get() != first.get() || !same_owner(first, second) || first.use_count() != 2) {
        return EXIT_FAILURE;
    }

    auto session = async_session::create("session-01");
    std::weak_ptr<async_session> observer = session;
    std::function<std::string()> callback = session->make_callback();
    session.reset();
    if (observer.expired() || !callback || callback() != "session-01") {
        return EXIT_FAILURE;
    }
    callback = {};
    return observer.expired() ? EXIT_SUCCESS : EXIT_FAILURE;
}
