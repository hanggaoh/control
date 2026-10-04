# Exercise 01 — C++20 RAII Unique Handle

## Scenario

实现一个可拥有 native resource 的 `unique_handle<Handle, Traits>`。它应适用于 socket 或 file handle；具体的 invalid value 和 close operation 由 `Traits` 提供。

这个练习关注 ownership，而不是 socket API 本身。测试使用 fake handle，因此结果 deterministic、快速且跨平台。

## Requirements

- `unique ownership`: 同一资源最多由一个 wrapper 拥有
- `non-copyable`: copy construction 和 copy assignment 在 compile time 被禁止
- `movable`: move construction 和 move assignment 转移 ownership
- `RAII`: destructor 对 valid handle 调用一次 `Traits::close`
- `exception safety`: destructor、move operations、`release`、`reset` 和 `swap` 均为 `noexcept`
- `release()`: 放弃 ownership，但不关闭资源
- `reset()`: 替换资源，并正确处理 empty state 与相同 handle
- `self-move assignment`: 不泄漏，也不产生 double-close

## Your task

只修改 [`include/unique_handle.hpp`](include/unique_handle.hpp) 中标有 `TODO` 的部分。不要修改 tests 来让失败消失。

建议按以下顺序推进：

1. destructor
2. `release()`
3. move constructor
4. `reset()`
5. move assignment

运行：

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

初始状态应当 build 成功、test 失败。这是预期的 red phase。

## Interview discussion prompts

准备解释这些问题：

1. 为什么 destructor 和 move operations 应该是 `noexcept`？
2. 为什么 `std::unique_ptr` 不能直接、自然地表示所有 native handles？
3. move assignment 应如何处理目标对象已拥有的资源？
4. `reset(get())` 为什么需要特别处理？
5. 如果 `Traits::close` 可能失败，destructor 应该如何设计？错误应该在哪里观察？
6. 这个 wrapper 是否满足 Rule of Five？哪些 special member functions 是显式定义或删除的？

## Optional extensions

- 添加 concrete POSIX `file_descriptor_traits`
- 添加 Windows `HANDLE` 与 `SOCKET` traits，并讨论它们不同的 invalid values / close functions
- 添加 `put()` 或 `out_ptr` 风格 API，供 C API 写入 handle
- 讨论 handle reuse、thread safety 与 close failure reporting
