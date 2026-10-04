#include "ui_boundary.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

using namespace interview::mfc::ui_practice;

class queued_dispatcher final : public ui_dispatcher {
public:
    void post(std::function<void()> work) override { work_.push_back(std::move(work)); }
    void drain() { for (auto& work : work_) work(); work_.clear(); }
private:
    std::vector<std::function<void()>> work_;
};

class recording_view final : public station_view {
public:
    void render(const station_snapshot& snapshot) override { last = snapshot; ++renders; }
    station_snapshot last;
    int renders{};
};

int main() {
    if (!exercise_complete) {
        std::cout << "SKIPPED (learner TODOs remain): MFC UI boundary exercise\n";
        return EXIT_SUCCESS;
    }

    queued_dispatcher dispatcher;
    auto view = std::make_shared<recording_view>();
    station_presenter presenter{dispatcher, view};
    presenter.on_worker_snapshot({"connected", 42.0});
    if (view->renders != 0) return EXIT_FAILURE;
    dispatcher.drain();
    if (view->renders != 1 || view->last.status != "connected" || view->last.value != 42.0) {
        return EXIT_FAILURE;
    }

    presenter.on_worker_snapshot({"late", 1.0});
    view.reset();
    dispatcher.drain(); // must safely no-op after window/view destruction
    return EXIT_SUCCESS;
}
