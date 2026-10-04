# System Design TODO — 90 Minutes

在 `solution/capstone-control-station` 完成下面的空白；完成后 review，再合并到 `answers`。
这是纸笔/Markdown 练习，答案留在 solution branch。

## Scenario and assumptions — 10 min

设计一个 Windows MFC operator station，通过 Boost.Asio TCP 接收 simulator telemetry、显示 alarm、发送 supervisory command，
并异步保存 telemetry/command audit。设备规模和时间约束由你提出，并说明假设。

- TODO: device count、sample rate、frame limit。
- TODO: acceptable telemetry age、command deadline、queue/disk capacity。
- TODO: 三项 functional requirements 和三项 quality requirements。

## Components and data flow — 20 min

- [ ] TODO: 画 telemetry → parser → core → database/UI 的组件图。
- [ ] TODO: 画 command → validation/interlock → transport → acknowledgement 的路径。
- [ ] TODO: 为 transport、repository 和 UI dispatcher 写接口 contract（文字即可）。
- [ ] TODO: 标出每条 queue 的容量与 overflow policy。

## Ownership and threads — 15 min

| Component | Owner | Execution context | Shared state protection | Destruction condition |
|---|---|---|---|---|
| TCP session/timers | TODO | TODO | TODO | TODO |
| parser/application state | TODO | TODO | TODO | TODO |
| control state | TODO | TODO | TODO | TODO |
| repository worker | TODO | TODO | TODO | TODO |
| presenter/subscription | TODO | TODO | TODO | TODO |
| MFC window/controls | TODO | TODO | TODO | TODO |

## Failure walkthrough — 25 min

为每一行补检测、transition、policy 和 UI/log 可见结果。

| Failure | Detection | State transition | Recovery/rejection policy | Observable result |
|---|---|---|---|---|
| connection lost | TODO | TODO | TODO | TODO |
| command timeout / unknown outcome | TODO | TODO | TODO | TODO |
| telemetry/write queue full | TODO | TODO | TODO | TODO |
| MFC window closes with pending callbacks | TODO | TODO | TODO | TODO |

- [ ] TODO: 写出关闭顺序，标明 cancel、notify、drain、join 和 destroy。
- [ ] TODO: 说明 late completion 如何避免重连复活或访问已销毁 UI。

## Trade-offs and explanation — 20 min

- [ ] TODO: 解释 strand vs mutex 的一个选择及其边界。
- [ ] TODO: 解释 strong vs weak callback capture 的一个选择及其 lifetime 代价。
- [ ] TODO: 给四个 failure scenarios 各指定一个验证方式。
- [ ] TODO: 用 90 秒讲解图、ownership 和 shutdown；英文概括控制在 4–6 句。

## Exit criteria

- [ ] 组件和双向数据流完整，framework/database boundary 清楚。
- [ ] 每个主要对象都有 owner 与执行线程，跨线程传递规则明确。
- [ ] 断网、超时、队列满、窗口关闭四种场景都有可检验的行为。
- [ ] 两个 trade-offs 有理由、成本和验证方式。

时间到后记录未解决的问题，继续下一个 session。参考架构用于 review，不能代替自己的闭卷设计。
