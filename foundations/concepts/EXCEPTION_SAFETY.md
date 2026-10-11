# Exception safety：清理资源与回滚状态

## 概念与 trade-offs

先写 invariant，再问异常发生在哪一步：

| Guarantee | 异常逃出操作后承诺什么 |
|---|---|
| Basic | invariant 保持成立，没有资源泄漏；值可能改变 |
| Strong | 对承诺保护的状态没有效果，相当于操作未发生 |
| No-throw | 异常不会逃出操作 |

“没有 leak”不能推出 strong guarantee。RAII 会销毁 temporary，但不会自动把业务状态恢复成旧值。
文件写入、日志、网络发送等外部副作用，也需要单独说明是否属于保证范围。

copy-and-swap 的思考顺序：

1. **Prepare**：构造候选状态；允许失败，但失败不能改变 target。
2. **Commit**：把候选状态交给 target；通常要求 `swap` 不抛异常。
3. **Cleanup**：temporary 现在持有旧资源，析构释放它；清理不能抛出异常。

假设对象有 `payload` 和 `size`，invariant 是 `size == payload.size()`。
若 swap 已交换 payload，交换 size 前抛异常，target 可能连 basic guarantee 都没有：
它的两个成员已经不匹配。不能把“strong 不成立”自动理解成“basic 一定成立”。

`noexcept` 是承诺，不是自动修复。若标记 noexcept 的 swap 实际让异常逃出，会调用 `std::terminate`。
确认所有成员交换都不抛异常，再作承诺。

copy-and-swap 容易推理，但可能复制整个对象、分配新内存，并放弃已有容量的复用。
大 buffer 的更新可以采用别的设计，但仍需明确失败前后状态；性能优化不能靠假定 rollback 自动发生。

## 用自己的话复述

请解释：“temporary 的析构能防止 leak，为什么仍不能撤销一半完成的 swap？”
画出 target/candidate 在 prepare、commit、cleanup 阶段各自拥有的资源。

## Runnable exercise 与验收

完成 [copy_swap](../exercises/copy_swap/README.md)，先读它的约束与现有测试，不看完整实现。
从仓库根目录运行：

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build -R foundation_copy_swap_exercise --output-on-failure
```

Review 重点：copy failure 后 target 是否不变？成功后旧资源是否只释放一次？
self-copy 是否安全？测试是否用确定的故障注入，而不是依赖真实内存耗尽？
TODO 状态的 SKIPPED 不等于练习通过。

## Interview follow-up

若新状态要同时更新内存与设备配置，你能承诺全局 strong guarantee 吗？如何界定外部副作用和失败恢复？

## Concise English interview answer

> Copy-and-swap separates preparation from commit. The strong guarantee depends on preparation leaving the target unchanged and commit being non-throwing. RAII cleans up resources; it does not automatically roll back partially changed state.
