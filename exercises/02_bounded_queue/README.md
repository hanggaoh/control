# Exercise 02 — Bounded Thread-Safe Queue

## Concept

`bounded_queue<T>` 是一个 blocking、multi-producer/multi-consumer queue。固定容量提供
`backpressure`：queue full 时 producer 阻塞，queue empty 时 consumer 阻塞。

`close()` 定义 lifecycle boundary：

- future `push()` 返回 `false`
- blocked producers 和 consumers 全部被唤醒
- 已经入队的数据仍然可以 drain
- closed 且 empty 后，`pop()` 返回 `std::nullopt`

实现位于 [`../../src/bounded_queue.hpp`](../../src/bounded_queue.hpp)，测试位于
[`../../tests/bounded_queue_tests.cpp`](../../tests/bounded_queue_tests.cpp)。

## Run

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

只运行本练习：

```sh
ctest --test-dir build -R bounded_queue --output-on-failure
```

## Review questions

1. 为什么 condition-variable predicate 必须同时检查 queue state 和 `closed_`？
2. 为什么 `close()` 必须对两个 condition variables 执行 `notify_all()`？
3. 为什么修改 shared state 时必须持有同一个 mutex？
4. 为什么在 `notify_one()` 之前先 unlock 通常更高效？
5. destructor 是否应该隐式调用 `close()`？它能否安全等待仍在访问 queue 的 threads？
