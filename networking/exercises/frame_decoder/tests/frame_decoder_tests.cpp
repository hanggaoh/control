#include "frame_decoder.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace interview::networking::frame_practice;

namespace {
std::vector<std::byte> bytes(std::initializer_list<unsigned int> values) {
    std::vector<std::byte> result;
    for (auto value : values) result.push_back(static_cast<std::byte>(value));
    return result;
}
}

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): frame decoder exercise\n";
        return EXIT_SUCCESS;
    }

    frame_decoder decoder{8};
    auto first = decoder.feed(bytes({0, 0}));
    auto second = decoder.feed(bytes({0, 3, 'a'}));
    auto third = decoder.feed(bytes({'b', 'c', 0, 0, 0, 1, 'z'}));
    if (!first.empty() || !second.empty() || third.size() != 2 ||
        third[0] != bytes({'a', 'b', 'c'}) || third[1] != bytes({'z'})) {
        return EXIT_FAILURE;
    }

    frame_decoder invalid{4};
    if (!invalid.feed(bytes({0, 0, 0, 5})).empty() || !invalid.failed()) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
