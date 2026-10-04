# Catapult Senior C++ Developer Interview Prep

这是 Hang Gao 的 hands-on C++ interview preparation repository，目标岗位侧重
industrial automation、Windows C++、Boost.Asio、networking、concurrency、legacy
modernization 和 high reliability。

教学采用：concept → short check → runnable exercise → senior-level review →
interview follow-up → concise English answer。代码与 technical vocabulary 使用英文，
解释使用中文。

## Exercises

1. [`01_unique_handle`](exercises/01_unique_handle/README.md) — C++20 RAII socket/file-handle wrapper
2. [`02_bounded_queue`](exercises/02_bounded_queue/README.md) — bounded multi-producer/multi-consumer queue

后续路线：

3. raw-pointer legacy refactor
4. TCP framing and partial reads
5. Boost.Asio async TCP lifecycle
6. Qt/MFC business-logic isolation
7. characterization tests and incremental modernization
8. SCADA/DNP3 reliability scenarios

## Build and test

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

第一个练习有意从 failing tests 开始。完成 TODO 后，目标是让测试全部通过。

只运行某个练习：

```sh
ctest --test-dir build -R exercise_01_unique_handle --output-on-failure
ctest --test-dir build -R bounded_queue --output-on-failure
```

## Start a learning session

在新的 Codex chat 中打开此目录，然后输入：

> Start exercise 1. Give me the requirements only. Review my solution as a Senior C++ interviewer.

项目约定和经历边界记录在 [`AGENTS.md`](AGENTS.md)。
