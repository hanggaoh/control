# Distributed Systems — 80/20 Core

## 1. Partial failure

本地 function 要么返回要么进程失败；remote operation 可能出现：请求未到、服务执行但 response 丢失、response 延迟、
双方状态不一致。timeout 只表示 caller 不再等待，不能证明 server 没执行。

## 2. Timeout, retry, backoff and jitter

所有 remote wait 都应有 timeout/cancellation policy。retry 只用于被判断为 transient 且可安全重试的 operation，使用 bounded
attempts、exponential backoff 和 jitter，避免 outage 时 synchronized retry storm。deadline 比每层独立重置 timeout 更能限制
端到端 latency。

## 3. Idempotency and deduplication

read 通常较容易 retry；command/write 可能重复产生 side effect。使用 request/idempotency key、deduplication storage 和明确
result semantics。exactly-once 通常不是简单 transport guarantee；更实用的是 at-least-once delivery + idempotent processing，
或 at-most-once + 可接受丢失，具体由业务决定。

## 4. Ordering and time

network 不保证不同连接或重试之间的全局 ordering。使用 per-source sequence、generation/session ID 和 domain rules 处理 stale、
duplicate、out-of-order messages。wall clock 会 skew/jump；duration/timeout 使用 monotonic clock，业务 timestamp 明确 source 和 UTC。

## 5. Consistency and concurrency

需要先说清 invariant：哪些数据必须立即一致，哪些允许 eventual consistency。optimistic version、transaction、conditional update
和 single-writer ownership 常比“全局 lock”更实际。CAP 不应被背成“三选二”口号；重点是在 partition 发生时系统选择什么行为。

## 6. Backpressure and overload

unbounded queue 把 overload 延迟成 memory failure。定义 capacity、reject/drop/coalesce policy，给 critical commands 与 replaceable
telemetry 不同待遇。load shedding、circuit breaker 和 health signal 都必须避免掩盖真实故障。

## 7. Observability and recovery

使用 correlation/idempotency/session IDs，记录 attempt、timeout、state transition 和 outcome。metrics 关注 latency distribution、error
rate、retry rate、queue depth 和 data age。recovery 需要 replay/reconciliation，而不只是“服务重新启动了”。

## 工业控制边界

remote SCADA command 不能替代 local protection/interlock。communication loss、stale data 和 split state 必须进入显式 operating mode。
本练习只讨论软件可靠性，不自行定义真实电力保护策略。

## 笔试前排除

- Paxos/Raft 证明与实现；
- distributed transaction protocol 深入实现；
- CRDT 数学细节；
- service mesh/Kubernetes 运维大全；
- global-scale sharding architecture。
