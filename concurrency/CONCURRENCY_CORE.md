# Concurrency Core — 80/20

## Tier 1 — 必须掌握

### Thread ownership and lifetime

每个 thread 都必须有 owner、stop mechanism 和 join point。避免 detached thread 捕获短生命周期对象。优先使用 RAII
thread ownership；`std::jthread` 的 stop token 是 cooperative cancellation，不会强制中断 blocking operation。

### Data race and invariant

两个 thread 并发访问同一 memory location，至少一个是 write 且没有 synchronization，就是 data race，行为未定义。
mutex 保护的是 invariant，不只是单个变量。先写清楚“哪些 state 必须一起改变”，再决定 lock scope。

### Mutex and RAII locking

- `lock_guard`：简单 scoped ownership；
- `unique_lock`：需要 unlock/relock 或配合 condition variable；
- 多 mutex 使用 consistent order 或 `scoped_lock`；
- 不在持锁时做不可控 I/O、callback 或长时间工作；
- callback 可能 re-enter 时尤其避免持锁调用外部代码。

### Condition variable

正确 mental model：shared state + mutex + predicate。永远使用 predicate wait：

```cpp
cv.wait(lock, [&] { return closed || !queue.empty(); });
```

predicate 同时处理 spurious wakeup、通知先于 wait、以及 shutdown。修改 predicate state 时持同一 mutex，通知只是提示
重新检查，不携带业务状态。

### Bounded queue and backpressure

明确 full/empty/closed：

- producer 在 full 时 block、timeout、drop 还是 reject；
- close 后是否允许 drain existing items；
- close 必须唤醒所有 blocked producers/consumers；
- push/pop 返回值要区分 timeout、closed 和 success；
- destructor 前先停止 users，不能让 waiter 使用已销毁的 mutex/CV。

### Shutdown

推荐顺序：stop accepting → publish stop/close state → notify waiters → cancel external operations → drain/exit workers → join →
destroy shared state。shutdown 应 idempotent，late work 不得重新启动系统。

## Atomics — 只学边界

第一轮只需掌握：atomic 适合独立 counter/flag；它不会自动保护跨多个变量的 invariant。理解 relaxed 只保证 atomicity，
acquire/release 可建立 publication ordering；不要在笔试前深入手写 lock-free structure 或复杂 memory-order proof。

## Common failure patterns

- check-then-act 分开加锁；
- wait 没有 predicate；
- shutdown 只设置 flag，没有 notify；
- thread 捕获 dangling `this`；
- lock order inversion；
- 持锁调用 user callback；
- 把 `volatile` 当 thread synchronization；
- 用 arbitrary sleep 让 flaky test “稳定”。

## 笔试前排除

- lock-free queue/stack；
- ABA problem 深入实现；
- hazard pointers/epoch reclamation；
- 全套 C++ memory model formalism；
- custom scheduler/work-stealing runtime。
