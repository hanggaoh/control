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

## D. SOLID quiz

1. **判断 — SRP**：一个类只要 method 少，就一定符合 Single Responsibility。
2. **单选 — OCP**：已有两种 retry policies 需要切换，最先评估：A. 分散条件分支；B. 小型 policy/callable 扩展点；
   C. 全系统改为 inheritance；D. global singleton。
3. **判断 — LSP**：fake transport 只要函数签名相同，即使 callback 线程和执行次数不同，也能安全替换 real transport。
4. **判断 — ISP**：只读 telemetry client 应依赖包含 admin/write/reset 等全部功能的接口。
5. **判断 — DIP**：在 C++ 中使用 templates/function objects 就无法实现 dependency inversion。

每题补一句理由，答案只写在 solution branch。

## E. SOLID senior questions and TODO audit

1. **SRP**：一个 `CDialog` 同时解析 TCP、更新 PID、写数据库和显示 alarm，按变化原因如何划分？哪些 state 应一起保留？
2. **OCP**：为 retry/interlock 选择一个最小扩展点，说明 runtime interface 与 callable/template policy 的取舍。
3. **LSP**：transport contract 规定指定 executor 上异步 completion 一次。设计 fake 的测试方案，覆盖 inline callback、
   cancellation 和 late completion。哪些改变会破坏替换性？
4. **ISP**：将巨大 `IDeviceService` 按 telemetry viewer、operator command sender、audit consumer 的需求整理接口。
5. **DIP**：给 `StationService` 注入 transport/repository/dispatcher，说明 reference、`unique_ptr` 的 ownership 差异和销毁顺序。

- [ ] TODO: 在 Capstone 图中标出 SRP/OCP/ISP/DIP 的一个具体落点。
- [ ] TODO: 为 transport 写 lifetime、error、thread、completion 和 cancellation contract。
- [ ] TODO: 列出两个 fake/real implementation 的 LSP contract tests。
- [ ] TODO: 用 60 秒解释一个 SOLID 改进及其新增成本。
