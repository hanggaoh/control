# Move selection：不是失败后重试

## 概念与 trade-offs

`std::move(x)` 改变表达式的 value category，本身不转移资源。
`std::move_if_noexcept(x)` 也不构造新对象，它根据 type traits 选择返回引用：

| `is_nothrow_move_constructible_v<T>` | `is_copy_constructible_v<T>` | 返回 type |
|---|---|---|
| true | 任意 | `T&&` |
| false | true | `const T&` |
| false | false | `T&&` |

```cpp
T destination(std::move_if_noexcept(source));
```

先选择表达式 type，随后初始化 destination 时才发生构造。即使返回 `T&&`，也不保证调用了一个真正转移
资源的 move constructor；例如只有 `const T&` copy constructor 的类型也可能接受 rvalue。
trait 判断的是表达式可构造性与异常性质，不是检测某个函数名字是否存在。

## 为什么 vector relocation 关心 noexcept

扩容需要建立新的 storage，并构造新元素。copy 可以在保留旧元素状态的前提下构造新元素；
若中途失败，销毁已构造的新元素、释放新 storage，旧元素仍可保留。

move 若不抛异常，就没有“移动第几个旧元素时抛异常”的失败点。若 move 可能抛异常，
源元素可能已经被修改：第 1 个成功移动，第 2 个改了源对象后抛出，此时旧 storage 不再等于原状态。
因此不能普遍采用 catch 后改用 copy 的恢复策略。

copy 可用时 vector 可以选择 copy 来满足异常保证；不要把这个解释说成标准要求内部必须调用
`std::move_if_noexcept`。不同操作有不同保证。特别是 `reserve`：当非 CopyInsertable 类型的 move
抛异常时，标准允许 effects unspecified，不能承诺 vector 内容或元素状态保持原样。

“可能抛异常但这次没有抛”不会改变编译期引用选择。`noexcept` 标记影响 generic code 的决策，
但只有实现确实不抛异常时才应使用；谎报会导致 terminate。

## 用自己的话复述

分别解释“选择引用”“调用构造函数”“释放旧 storage”发生在哪个阶段。
为什么 modified source 使“move 失败后再 copy”不可靠？

## Runnable exercise 与验收

完成 [move_lifetime](../exercises/move_lifetime/README.md)。

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build -R foundation_move_lifetime_exercise --output-on-failure
```

Review 重点：用 traits/static_assert 检查引用类型；用计数器观察后续 copy/move；
copy failure 与 throwing move 分开推理。不要对 unspecified effects 写固定结果断言。
TODO 状态的 SKIPPED 不等于完成。

## Interview follow-up

一个 move-only 大 buffer 类型需要在 move 中进行可能失败的设备调用。你会如何拆分资源转移与设备操作？

## Concise English interview answer

> move_if_noexcept selects a reference type using compile-time traits. It does not copy or retry. A container can prefer copying when moving may throw, because a failed move may already have modified its source.
