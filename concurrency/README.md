# Concurrency — 80/20 Core

并发是 P0，但范围严格限制为应用层 correctness。第一轮总时间：一天；唯一必须完成的核心代码是仓库已有的
`bounded_queue`，不新增 lock-free exercise。

1. 阅读 [`CONCURRENCY_CORE.md`](CONCURRENCY_CORE.md)。
2. 完成 [`EXERCISES.md`](EXERCISES.md)。
3. 完成并 review `src/bounded_queue.hpp` 与 `tests/bounded_queue_tests.cpp`。
4. 能画出 producer、consumer、close/shutdown 的 happens-before 与 lifetime。

答案放在 `solution/concurrency`，review 后合并到 `answers`。

## 一天时间盒

| Session | 内容 |
|---|---|
| 1 | data race、mutex、RAII locks、invariant |
| 2 | condition variable、predicate、spurious/lost wakeup |
| 3 | bounded queue coding/tests、close semantics |
| 4 | deadlock、shutdown、atomics boundary、口头解释 |
