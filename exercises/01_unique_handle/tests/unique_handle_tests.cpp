#include "unique_handle.hpp"

#include <cstdlib>
#include <iostream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

struct fake_handle_traits {
    static constexpr int invalid() noexcept { return -1; }

    static void close(int handle) noexcept { closed_handles.push_back(handle); }

    static inline std::vector<int> closed_handles;
};

using test_handle = interview::unique_handle<int, fake_handle_traits>;

static_assert(!std::is_copy_constructible_v<test_handle>);
static_assert(!std::is_copy_assignable_v<test_handle>);
static_assert(std::is_nothrow_move_constructible_v<test_handle>);
static_assert(std::is_nothrow_move_assignable_v<test_handle>);
static_assert(std::is_nothrow_destructible_v<test_handle>);

class test_context {
public:
    void check(bool condition, std::string_view expression, int line) {
        if (!condition) {
            ++failures_;
            std::cerr << "line " << line << ": CHECK(" << expression << ") failed\n";
        }
    }

    [[nodiscard]] int failures() const noexcept { return failures_; }

private:
    int failures_{0};
};

#define CHECK(expression) context.check(static_cast<bool>(expression), #expression, __LINE__)

void clear_closed_handles() { fake_handle_traits::closed_handles.clear(); }

void default_construction_is_empty(test_context& context) {
    test_handle handle;

    CHECK(!handle);
    CHECK(handle.get() == fake_handle_traits::invalid());
}

void destructor_closes_an_owned_handle_once(test_context& context) {
    clear_closed_handles();
    {
        test_handle handle{10};
        CHECK(handle);
    }

    CHECK(fake_handle_traits::closed_handles == std::vector<int>{10});
}

void move_construction_transfers_ownership(test_context& context) {
    clear_closed_handles();
    {
        test_handle source{20};
        test_handle destination{std::move(source)};

        CHECK(!source);
        CHECK(destination.get() == 20);
    }

    CHECK(fake_handle_traits::closed_handles == std::vector<int>{20});
}

void move_assignment_closes_old_resource_and_transfers(test_context& context) {
    clear_closed_handles();
    {
        test_handle source{30};
        test_handle destination{31};

        destination = std::move(source);

        CHECK(!source);
        CHECK(destination.get() == 30);
        CHECK(fake_handle_traits::closed_handles == std::vector<int>{31});
    }

    CHECK(fake_handle_traits::closed_handles == std::vector<int>({31, 30}));
}

void self_move_assignment_preserves_single_ownership(test_context& context) {
    clear_closed_handles();
    {
        test_handle handle{40};
        auto move_from_self = [&handle]() -> test_handle&& { return std::move(handle); };
        handle = move_from_self();
        CHECK(handle.get() == 40);
    }

    CHECK(fake_handle_traits::closed_handles == std::vector<int>{40});
}

void release_returns_handle_without_closing(test_context& context) {
    clear_closed_handles();
    int raw = fake_handle_traits::invalid();
    {
        test_handle handle{50};
        raw = handle.release();

        CHECK(raw == 50);
        CHECK(!handle);
    }

    CHECK(fake_handle_traits::closed_handles.empty());
}

void reset_replaces_and_closes_owned_handle(test_context& context) {
    clear_closed_handles();
    {
        test_handle handle{60};
        handle.reset(61);
        CHECK(handle.get() == 61);
        CHECK(fake_handle_traits::closed_handles == std::vector<int>{60});

        handle.reset();
        CHECK(!handle);
        CHECK(fake_handle_traits::closed_handles == std::vector<int>({60, 61}));
    }

    CHECK(fake_handle_traits::closed_handles == std::vector<int>({60, 61}));
}

void reset_with_same_handle_is_a_no_op(test_context& context) {
    clear_closed_handles();
    {
        test_handle handle{70};
        handle.reset(handle.get());
        CHECK(handle.get() == 70);
        CHECK(fake_handle_traits::closed_handles.empty());
    }

    CHECK(fake_handle_traits::closed_handles == std::vector<int>{70});
}

void swap_exchanges_ownership(test_context& context) {
    test_handle left{80};
    test_handle right{81};

    using std::swap;
    swap(left, right);

    CHECK(left.get() == 81);
    CHECK(right.get() == 80);
}

}  // namespace

int main() {
    test_context context;

    default_construction_is_empty(context);
    destructor_closes_an_owned_handle_once(context);
    move_construction_transfers_ownership(context);
    move_assignment_closes_old_resource_and_transfers(context);
    self_move_assignment_preserves_single_ownership(context);
    release_returns_handle_without_closing(context);
    reset_replaces_and_closes_owned_handle(context);
    reset_with_same_handle_is_a_no_op(context);
    swap_exchanges_ownership(context);

    if (context.failures() != 0) {
        std::cerr << context.failures() << " check(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "all checks passed\n";
    return EXIT_SUCCESS;
}
