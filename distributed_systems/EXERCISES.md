# Distributed Systems Scenario Exercises

答案只进入 `solution/distributed-systems` 与 `answers`。

1. Client 发送“close breaker”命令后 timeout。为什么不能直接断言命令失败？重试前需要哪些 identity、deduplication 和
   reconciliation 设计？不要自行定义该命令在真实系统中的 safety authorization。
2. 1000 个 clients 同时断线。固定每秒 retry 有什么问题？设计 bounded exponential backoff + jitter。
3. telemetry sequence 收到 `100, 101, 101, 99, 103`。分别识别 duplicate、stale 和 gap，并说明记录/处理策略。
4. queue 已满时，为什么 telemetry snapshot 可能 coalesce，而 operator command 不应静默 drop？
5. database commit 成功但 acknowledgement 丢失。解释 outcome unknown、idempotency key 和查询最终状态。
6. 比较 at-most-once、at-least-once 与“业务上的 exactly-once effect”。
7. local controller 与 SCADA 断联。哪些功能必须本地继续，哪些 remote operations 应冻结或降级？
8. 画出 request deadline 在 UI → service → TCP → database 多层调用中的传播，避免每层重新获得完整 timeout。
