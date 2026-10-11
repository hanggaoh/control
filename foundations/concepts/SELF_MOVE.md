# Self-move：先识别 aliasing

## 概念与 trade-offs

在 `x = std::move(x)` 中，`std::move` 不创建第二个对象。
move assignment 内的 `this` 和 `&other` 指向同一个 owner。

假设 owner 持有 handle 42：如果先关闭目标，再从 other 读取 handle，那么读到的仍是同一个对象。
若 release 清空成员，可能得到空对象；若没有清空，可能重新保存已关闭的 handle，之后 double close。
“先接管再释放 other”也不是通用修复：self-move 时释放 other 就是释放自己的资源。

设计必须先规定 contract：

- **Self-move no-op**：识别 `this == &other`，不改变资源。简单，容易测试。
- **允许 self-move 后为空**：也可以是合法的自定义 contract，但仍必须 valid、无泄漏、无 double close。
- **Temporary-and-swap**：先转移到 temporary，再交换；必须结合 move constructor、非抛出 swap 与析构
  整体证明。不能只凭使用了 swap 就认为安全。

标准库类型通常允许 self-move 后处于 valid but unspecified state；这不等于保持原值。
自定义 unique owner 可承诺更强的 no-op，务必在接口与测试中一致表达。

`std::exchange` 能取旧值并写入空状态，但不是 aliasing 自动修复器；仍需检查整个操作顺序。
先确认谁 owns handle，再决定哪一步可以 close。

## 用自己的话复述

把 `this` 与 `other` 画成同一个方框，逐步说明 release 和 transfer 修改了什么。
为何“两次不同的变量名”不代表“两份独立资源”？

## Runnable exercise 与验收

完成 [unique_resource](../exercises/unique_resource/README.md)，遵循它的 self-move contract。

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build -R foundation_unique_resource_exercise --output-on-failure
```

Review 重点：空 owner、两个非空 owner、self-move、重复 reset、析构；
使用 fake closer 记录每个 handle 的释放次数，不用真实 socket 的偶然行为证明正确。
TODO 状态的 SKIPPED 不等于完成。

## Interview follow-up

如果 close 失败，你会让 destructor 或 noexcept move assignment 抛异常吗？怎样记录失败并避免重复释放？

## Concise English interview answer

> Self-move is an aliasing case: source and destination are the same object. I define the contract explicitly and verify that assignment preserves a valid state without leaks or double release. For a unique resource wrapper, a self-assignment check is a simple no-op policy.
