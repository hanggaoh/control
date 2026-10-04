# C++ Idioms and Practical Patterns — Exercises

答案只进入 `solution/idioms-patterns` 与 `answers`。

## A. 判断与选择

1. **判断**：只要 class 包含 raw pointer，就一定应该手写 Rule of Five。
2. **单选**：scope-exit transaction guard 的 destructor 默认行为应是：A. commit；B. rollback；C. retry；D. detach。
3. **判断**：PImpl 总能提高 runtime performance，且没有额外 lifetime/copy design cost。
4. **单选**：把 MFC view 调用转换成 pure core command 最接近：A. Adapter；B. Singleton；C. Prototype；D. Flyweight。
5. **判断**：Observer 的主要问题只是如何遍历 subscriber list，与 lifetime/thread affinity 无关。
6. **单选**：connection 有 Connecting/Connected/Stopping/Stopped transitions，最先考虑：A. explicit State model；
   B. scattered booleans；C. global singleton；D. macro。
7. **判断**：使用 Strategy 一定需要 virtual base class 和 heap allocation。
8. **单选**：异步 session callback 要延长已有 shared ownership，应使用：A. `shared_ptr(this)`；
   B. `shared_from_this()`；C. raw `this`；D. detached pointer。

## B. Senior scenario questions

1. 一个 resource wrapper 应该选择 Rule of Zero、move-only Rule of Five 还是 copyable value semantics？说明判断过程。
2. copy-and-swap 的 strong guarantee 来自哪些步骤？什么 performance trade-off 可能让你选择另一实现？
3. 为 Boost.Asio transport 设计 Adapter interface，哪些 framework types 不应泄漏到 core？
4. MFC UI 订阅 telemetry 时，Observer 如何处理 unsubscribe、window destruction 和 UI-thread affinity？
5. 用 enum + transition function 与 State class hierarchy 分别实现 connection state，有什么取舍？
6. retry policy 应使用 runtime Strategy、template policy 还是普通 function object？根据什么选择？
7. PImpl 如何影响 destructor、copy/move、ABI、allocation 和 build time？
8. 将 operator command 建模为 Command value 有什么 audit/idempotency/testing 收益？

## C. Capstone pattern audit

在 `capstone/ARCHITECTURE.md` 中标出 Strategy、State、Adapter、Observer、Command 和 Factory。对每一个回答：

- 它隔离了哪个变化轴？
- ownership/lifetime 谁负责？
- 最简单的可行实现是什么？
- 如果移除这个 pattern，会出现什么 concrete coupling 或 failure？
