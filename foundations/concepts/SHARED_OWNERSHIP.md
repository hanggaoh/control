# Shared ownership：address 与 ownership identity

## 概念与 trade-offs

`shared_ptr` 的 mental model 包括 stored pointer 和 control block。
control block 管理 strong ownership、weak observation、managed object 的销毁策略等。
最后一个 strong owner 消失时销毁 managed object；weak_ptr 可让 control block 继续存在，直到最后的 weak
observation 消失。对于 make_shared 的合并分配，对象销毁后分配的 storage 也可能继续保留。

复制已有 shared_ptr 共享 control block。从同一个 raw pointer 分别构造 shared_ptr 则创建两个独立
ownership groups；默认删除器下可能 double delete。`use_count()` 只描述当前 group，不能证明不存在另一组。

反方向也成立：aliasing constructor 可以共享 control block，但 stored pointer 指向 owner 内的不同成员。
C++20 用 ownership ordering 判断同组：`!a.owner_before(b) && !b.owner_before(a)`；
它对 empty owners 也适用，不能据此推出对象一定存在。`get()` 比较的是地址，不是 ownership。

## shared_from_this 的前置条件

类型 public、无歧义地继承 `std::enable_shared_from_this<T>`，并且对象已由合适的 shared_ptr 建立 ownership。
`shared_from_this()` 从已建立的 control block 取得新的 owner，不是创建第二组。
在普通 make_shared 构造流程的 constructor 内，shared ownership 尚未接好；调用会抛 `std::bad_weak_ptr`。
stack object 也没有这个前提。可使用 factory，在构造完成并建立 ownership 后执行 start。

`weak_from_this()` 本身不会因为没有 owner 而抛异常，但会得到 empty weak_ptr。
constructor 内保存的 empty weak_ptr 不会在稍后自动变成有效观察者。

## Async lifetime 与 cancellation

- 捕获 strong `self`：callback 持有 owner，延长 lifetime，直到该 capture 被销毁。可能也延长资源占用。
- 捕获 weak：callback 执行时 `lock()`；成功得到的本地 shared_ptr 在使用期间保活，失败就跳过。
- 捕获 raw `this`：不保活，需要独立的 lifetime contract。

strong capture 不自动取消操作，也不保证 callback 不并发。shared_ptr 的计数安全不能保护对象里的业务成员。
对象自己持有一个捕获 self 的 callback 时可能形成 cycle；需要明确释放 callback 的路径或使用 weak。
weak capture 允许对象提前销毁，但不自动取消底层 I/O，shutdown 仍需单独设计。

## 用自己的话复述

为何 `shared_ptr<T>(this)` 与 `shared_from_this()` 在 `get()` 相同时仍有不同风险？
用 ownership 箭头画出 session → stored callback → session 的 cycle。

## Runnable exercise 与验收

完成 [shared_ownership](../exercises/shared_ownership/README.md)。

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build -R foundation_shared_ownership_exercise --output-on-failure
```

Review 重点：ownership identity、strong/weak callback 的析构时机、取消路径。
使用 fake executor 显式推进 callback；不要实际执行 double-delete 示例来“测试风险”。
TODO 状态的 SKIPPED 不等于完成。

## Interview follow-up

shutdown 后 callback 仍到达，strong self 仍 alive。如何确保它不再更新已关闭的业务状态？

## Concise English interview answer

> shared_from_this shares an existing control block; constructing shared_ptr from this creates another ownership group. Strong captures extend lifetime, while weak captures allow work to be skipped after destruction. Neither replaces cancellation or synchronization.
