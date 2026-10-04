# Control Systems — Conceptual Exercises

答案只保存在 `solution/control-systems` 和最终 `answers` branch，不进入 `main`。

## A. 选择题与判断题

1. **判断**：PID output 被 clamp 后，只要最终 command 在范围内，integral state 是否继续增长都无所谓。
2. **单选**：减小 low-pass filter 的 `alpha` 通常会：A. 减少平滑且减少延迟；B. 增加平滑也增加延迟；
   C. 消除所有 phase lag；D. 自动保证 closed-loop stability。
3. **判断**：hysteresis 的 ON 和 OFF threshold 应完全相同，才能避免 chatter。
4. **单选**：network connection 丢失时，fast local protection 最合理的依赖是：A. 等待 SCADA command；
   B. 本地独立执行既定保护逻辑；C. 忽略 fault；D. 无限重试日志上传。
5. **判断**：在 fixed-step simulation 中 tuning 正常，就能证明面对 scheduler jitter 和 stale measurements 仍稳定。
6. **单选**：manual → automatic 的 command jump 主要应通过什么概念处理？A. bumpless transfer；
   B. 更大的 derivative gain；C. 删除 output limits；D. unordered execution。

## B. Senior interview 简答题

1. 描述 integrator windup 的形成过程。比较 conditional integration、clamping 和 back-calculation。
2. 为什么 derivative term 对 noise 敏感？对 measurement 求导与对 error 求导有什么行为差异？
3. 设计一个 pump start state machine：列出 states、interlocks、timeouts、fault latch 和 reset authority。
4. 区分 protection、local closed-loop control、PLC sequence 与 SCADA supervisory control。
5. controller 收到 NaN、out-of-range 或 stale measurement 时应怎样处理？哪些行为必须由 safety requirements 决定？
6. 如何测试 PID 实现，而不把“unit tests 通过”错误等同于“真实 plant 一定稳定”？
7. C++ periodic control loop 中，allocation、locking、logging 和 exception 分别可能造成什么 timing risk？

## C. Scenario drill

一个远端 setpoint 通过网络到达 local controller。说明从 parsing 到 actuation 之间需要哪些 validation、authorization、
range/rate limiting、interlock、audit 和 fallback。指出哪些步骤属于 software correctness，哪些需要 domain/safety owner 决策。
