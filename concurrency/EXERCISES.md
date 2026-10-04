# Concurrency Exercises

答案只进入 `solution/concurrency` 与 `answers`。

## A. 判断与选择

1. **判断**：`volatile bool stop` 可以安全地在线程间传播 shutdown。
2. **单选**：condition-variable wait 最重要的配套是：A. sleep；B. predicate + mutex-protected state；
   C. atomic counter only；D. detached thread。
3. **判断**：notify 发生在 worker 开始 wait 之前时，predicate wait 仍可观察已经改变的 state。
4. **单选**：bounded queue close 时最合理的是：A. 只设置 flag；B. notify blocked producers/consumers；
   C. detach waiters；D. 销毁 mutex。
5. **判断**：每个字段都是 atomic，就意味着跨字段 invariant 自动一致。
6. **单选**：避免双 mutex deadlock 的常用办法是：A. random sleep；B. consistent order/`scoped_lock`；
   C. recursive retry；D. `volatile`。

## B. Senior 简答

1. 用 shared state、mutex、predicate、notification 解释 condition variable，不要把 notification 当消息队列。
2. 设计 bounded queue 的 `close()`：blocked push/pop、existing items 和重复 close 分别怎样处理？
3. 为什么持锁调用 callback 危险？说明 reentrancy、latency 和 lock-order 风险。
4. 比较 mutex-protected flag、atomic flag 和 stop token 的适用范围。
5. 如何写 deterministic concurrency test，避免依赖 `sleep_for` 猜测 scheduling？
6. 解释一个 clean worker shutdown 的完整 lifetime 顺序。
