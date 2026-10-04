# Capstone Acceptance Criteria

## Functional

- 能连接 simulator、解析 fragmented telemetry、更新 UI snapshot；
- operator command 经过 validation/interlock 后发送，并显示 ack/rejection/timeout；
- filter/hysteresis/control simulation 可配置、可 reset、可 replay；
- disconnect 后按 bounded backoff reconnect，manual stop 后不再 reconnect。

## Correctness and safety

- oversized/malformed frame 不造成越界或 unbounded allocation；
- stale/invalid telemetry 不进入 control decision；
- 同一 session 不存在 concurrent read chain 或 interleaved frame writes；
- callback 不使用 dangling `this`；control block ownership 清晰；
- queue overload、communication loss、shutdown 都有显式 policy；
- MFC controls 只在 UI thread 访问；shutdown 可重复调用。

## Tests

- pure core unit tests 不依赖 Boost/MFC、network 或 wall-clock sleeps；
- parser 覆盖 byte-by-byte fragmentation、multiple frames、invalid length/version；
- control tests 覆盖 saturation、anti-windup、noise、threshold boundary、reset；
- transport tests 使用 loopback/fake clock（可行时）验证 timeout/cancellation/reconnect；
- integration scenario 可 deterministic replay；
- sanitizer/static analysis 可用的环境中不报告 leak、use-after-free 或 data race。

## Maintainability

- framework headers 不进入 domain/application public interfaces；
- ownership 和 thread affinity 可从 types/API/documentation 判断；
- error translation、logging fields 和 state transitions 集中定义；
- 新 protocol message 或 UI view 不要求修改无关 components。

## Demonstration

面试演示应在 10 分钟内展示：正常 telemetry、partial frame、command validation、disconnect/reconnect、alarm/hysteresis，
最后关闭窗口并证明 clean shutdown。随后能解释 architecture、failure paths、tests 和一个刻意选择的 trade-off。
