#pragma once

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <utility>

namespace interview {

// A blocking multi-producer, multi-consumer queue with a fixed capacity.
//
// close() prevents future pushes and wakes all waiting threads. Consumers may
// continue to drain values that were queued before close(). Once closed and
// empty, pop() returns std::nullopt.
template <typename T>
class bounded_queue {
public:
    explicit bounded_queue(std::size_t capacity) : capacity_(capacity) {
        if (capacity == 0) {
            throw std::invalid_argument{"bounded_queue capacity must be positive"};
        }
    }

    bounded_queue(const bounded_queue&) = delete;
    bounded_queue& operator=(const bounded_queue&) = delete;
    bounded_queue(bounded_queue&&) = delete;
    bounded_queue& operator=(bounded_queue&&) = delete;

    // Blocks while the queue is full. Returns false if close() wins the race.
    [[nodiscard]] bool push(T value) {
        std::unique_lock lock{mutex_};
        not_full_.wait(lock, [this] { return queue_.size() < capacity_ || closed_; });

        if (closed_) {
            return false;
        }

        queue_.push_back(std::move(value));
        lock.unlock();
        not_empty_.notify_one();
        return true;
    }

    // Blocks while empty. A closed queue is drained before nullopt is returned.
    [[nodiscard]] std::optional<T> pop() {
        std::unique_lock lock{mutex_};
        not_empty_.wait(lock, [this] { return !queue_.empty() || closed_; });

        if (queue_.empty()) {
            return std::nullopt;
        }

        T value = std::move(queue_.front());
        queue_.pop_front();
        lock.unlock();
        not_full_.notify_one();
        return value;
    }

    // Idempotent. Wakes both blocked producers and blocked consumers.
    void close() noexcept {
        {
            std::lock_guard lock{mutex_};
            closed_ = true;
        }
        not_empty_.notify_all();
        not_full_.notify_all();
    }

    [[nodiscard]] bool is_closed() const noexcept {
        std::lock_guard lock{mutex_};
        return closed_;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        std::lock_guard lock{mutex_};
        return queue_.size();
    }

private:
    const std::size_t capacity_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    std::deque<T> queue_;
    bool closed_{false};
};

}  // namespace interview
