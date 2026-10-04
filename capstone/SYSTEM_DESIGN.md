# System Design — Boost.Asio + MFC Integration

本章节将已学内容组织成一个可解释的系统。第一轮只需一个 90-minute session，练习见
[`SYSTEM_DESIGN_TODO.md`](SYSTEM_DESIGN_TODO.md)。参考架构见 [`ARCHITECTURE.md`](ARCHITECTURE.md)。

## 1. 从 requirements 开始

先明确设备数量、telemetry rate、最大 frame、允许的 data age、command deadline、queue capacity 和 shutdown 时间目标。
不知道的数值要列为 assumption。区分 functional requirements（系统做什么）与 quality requirements（多久完成、失败后怎么办）。
设计容量可以先用 `devices × samples/second × bytes/sample` 估算；写明峰值和余量，避免凭感觉设置无限队列。

## 2. Component and interface boundaries

transport 负责 bytes 和 connection lifecycle；parser 将 bytes 转为 typed messages；application 负责协调；domain/control
负责业务 invariant；repository 负责 persistence；presenter/dispatcher 负责把 snapshot 送到 UI thread。

接口同时表达 success/error、ownership、thread affinity 和 cancellation contract。framework types 留在 adapter。
数据库写入安排在独立任务/worker，避免在 UI thread 或 Asio handler 中执行慢查询。

## 3. Ownership and execution context

对每个 stateful component 回答：谁创建、谁销毁、在哪个 executor/thread 修改、哪些 callback 可能比 caller 活得更久。
共享 lifetime 与共享 mutable state 是两个问题：`shared_ptr` 管 lifetime，mutex/strand 管 state access。

strand 串行化投递到同一 strand 的 handlers，适合 session state；mutex 适合需要跨 execution contexts 同步访问的 state。
两者都需要明确边界。strong capture 延长对象 lifetime；weak capture 允许 owner 消失后跳过工作；capture policy 来自 operation contract。

## 4. Data flow and bounded queues

telemetry 路径：device → TCP → parser → validation/control → persistence/snapshot → MFC。
command 路径：MFC → application → validation/interlock → ordered write → acknowledgement/result。

每条 queue 都要定义容量、producer/consumer、overflow policy 和 shutdown behavior。最新 telemetry 可以按需求 coalesce；
command 必须反馈拒绝或失败。disk backlog 同样需要容量与 retention。snapshot 应记录 sequence、quality 和 timestamps。

## 5. Failure is part of the interface

断网会影响 session 和 data freshness；timeout 不能证明 remote command 未执行；retry 要结合 idempotency 和 deadline。
重连后的旧 completion 需要 generation/state check。窗口关闭涉及 subscriber invalidation、cancellation、worker join 和 destruction。

推演故障时按四步回答：检测信号 → state transition → 恢复/拒绝策略 → 可观察结果。用日志和 metrics 解释如何确认系统恢复。

## 6. Testability and trade-offs

用 fake transport、clock、repository、dispatcher 验证 core；用 loopback integration 验证网络 adapter；单独验证 UI-thread boundary。
选择一个 design decision，写清 requirement、两个候选方案、成本和验证方式。常见比较包括 strand/mutex、strong/weak capture、
batch/immediate persistence、drop/coalesce/block、single/multiple I/O threads。

第一轮完成标准只有四项：组件/数据流、ownership/thread map、四种故障推演、两个有理由的取舍。
