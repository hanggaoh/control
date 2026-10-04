#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>

namespace interview::mfc::ui_practice {

inline constexpr bool exercise_complete = false; // TODO: set true when complete.

struct station_snapshot {
    std::string status;
    double value{};
};

class ui_dispatcher {
public:
    virtual ~ui_dispatcher() = default;
    virtual void post(std::function<void()> work) = 0;
};

class station_view {
public:
    virtual ~station_view() = default;
    virtual void render(const station_snapshot& snapshot) = 0;
};

class station_presenter {
public:
    station_presenter(ui_dispatcher& dispatcher, std::weak_ptr<station_view> view)
        : dispatcher_(dispatcher), view_(std::move(view)) {}

    void on_worker_snapshot(station_snapshot snapshot) {
        // TODO: post work through dispatcher. Capture snapshot by value and
        // lock the weak view only when dispatched; never call render here.
        (void)snapshot;
    }

private:
    ui_dispatcher& dispatcher_;
    std::weak_ptr<station_view> view_;
};

}  // namespace interview::mfc::ui_practice
