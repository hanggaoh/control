#include "bounded_queue.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <future>
#include <iostream>
#include <latch>
#include <mutex>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <thread>
#include <type_traits>
#include <vector>

namespace {

using namespace std::chrono_literals;

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

using int_queue = interview::bounded_queue<int>;

static_assert(!std::is_copy_constructible_v<int_queue>);
static_assert(!std::is_move_constructible_v<int_queue>);
static_assert(noexcept(std::declval<int_queue&>().close()));

void rejects_zero_capacity(test_context& context) {
    bool threw = false;
    try {
        int_queue queue{0};
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    CHECK(threw);
}

void preserves_fifo_order(test_context& context) {
    int_queue queue{3};
    CHECK(queue.push(1));
    CHECK(queue.push(2));
    CHECK(queue.push(3));
    CHECK(queue.pop() == 1);
    CHECK(queue.pop() == 2);
    CHECK(queue.pop() == 3);
}

void close_wakes_a_blocked_consumer(test_context& context) {
    int_queue queue{1};
    std::promise<void> started;
    auto started_future = started.get_future();
    auto result = std::async(std::launch::async, [&] {
        started.set_value();
        return queue.pop();
    });

    started_future.wait();
    queue.close();

    CHECK(result.wait_for(1s) == std::future_status::ready);
    CHECK(!result.get().has_value());
}

void close_wakes_a_blocked_producer(test_context& context) {
    int_queue queue{1};
    CHECK(queue.push(1));

    std::promise<void> started;
    auto started_future = started.get_future();
    auto result = std::async(std::launch::async, [&] {
        started.set_value();
        return queue.push(2);
    });

    started_future.wait();
    queue.close();

    CHECK(result.wait_for(1s) == std::future_status::ready);
    CHECK(!result.get());
}

void close_allows_queued_values_to_be_drained(test_context& context) {
    int_queue queue{2};
    CHECK(queue.push(10));
    CHECK(queue.push(11));
    queue.close();

    CHECK(queue.is_closed());
    CHECK(!queue.push(12));
    CHECK(queue.pop() == 10);
    CHECK(queue.pop() == 11);
    CHECK(!queue.pop().has_value());

    queue.close();
    CHECK(queue.is_closed());
}

void supports_multiple_producers_and_consumers(test_context& context) {
    constexpr int producer_count = 4;
    constexpr int consumer_count = 3;
    constexpr int values_per_producer = 500;
    constexpr int total_values = producer_count * values_per_producer;

    int_queue queue{17};
    std::latch start_gate{producer_count + consumer_count};
    std::mutex results_mutex;
    std::vector<int> results;
    results.reserve(total_values);
    std::vector<std::jthread> consumers;
    std::vector<std::jthread> producers;

    for (int i = 0; i < consumer_count; ++i) {
        consumers.emplace_back([&] {
            start_gate.arrive_and_wait();
            while (auto value = queue.pop()) {
                std::lock_guard lock{results_mutex};
                results.push_back(*value);
            }
        });
    }

    for (int producer = 0; producer < producer_count; ++producer) {
        producers.emplace_back([&, producer] {
            start_gate.arrive_and_wait();
            const int first = producer * values_per_producer;
            for (int offset = 0; offset < values_per_producer; ++offset) {
                if (!queue.push(first + offset)) {
                    return;
                }
            }
        });
    }

    for (auto& producer : producers) {
        producer.join();
    }
    queue.close();
    for (auto& consumer : consumers) {
        consumer.join();
    }

    std::ranges::sort(results);
    CHECK(results.size() == total_values);
    CHECK(std::ranges::equal(results, std::views::iota(0, total_values)));
    CHECK(queue.size() == 0);
}

}  // namespace

int main() {
    test_context context;

    rejects_zero_capacity(context);
    preserves_fifo_order(context);
    close_wakes_a_blocked_consumer(context);
    close_wakes_a_blocked_producer(context);
    close_allows_queued_values_to_be_drained(context);
    supports_multiple_producers_and_consumers(context);

    if (context.failures() != 0) {
        std::cerr << context.failures() << " check(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "all bounded_queue checks passed\n";
    return EXIT_SUCCESS;
}
