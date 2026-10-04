# Control Systems for Industrial and Power C++

## 1. 先分清系统层级

工业/电力系统中的“控制”不是一个单一 loop：

| 层级 | 典型职责 | 典型时间尺度 |
|---|---|---|
| protection | fault detection、trip，优先保证设备与人员安全 | very fast，且高度确定性 |
| local control | drive/governor/regulator/PID，直接闭环 | fast periodic loop |
| PLC/sequence control | interlock、state machine、startup/shutdown sequence | scan/event based |
| SCADA supervisory | monitoring、alarm、operator command、setpoint dispatch | slower，networked |
| optimization/planning | scheduling、economic dispatch、offline analysis | seconds to hours |

具体时间尺度取决于设备和标准，不能把表格当硬编码要求。关键是：网络中断时，本地 safety/protection 不能依赖
远端 SCADA 才能工作；supervisory command 也必须经过 validation、authorization 和 local interlock。

## 2. Feedback mental model

closed loop 的基本关系是：setpoint `r` 与 measurement `y` 形成 error `e = r - y`，controller 产生 command
`u`，plant 和 disturbances 决定下一时刻的 output。讨论任何算法都要明确：

- signal units 和 sign convention；
- measurement 是否新鲜、有效、带 timestamp；
- actuator limits、rate limits 和 deadband；
- controller execution period 与 jitter；
- sensor/actuator failure 时进入什么 safe state。

稳定性不是“测试几个输入没崩溃”。closed-loop poles、gain/phase margin、delay 和 sampling 都会改变动态行为。

## 3. Continuous 到 discrete

C++ controller 实际运行在 sample instants。离散积分的简单形式：

```text
I[k] = I[k-1] + e[k] * dt
```

derivative 可写成 `(e[k] - e[k-1]) / dt`，但它会放大 measurement noise，并可能在 setpoint step 时产生
derivative kick。工程实现常对 derivative filtering，或对 measurement 而不是 error 求导。

`dt` 必须来自明确 contract：固定周期 loop 可使用 configured sample time；若 scheduler jitter 不能忽略，应测量
实际 elapsed time、设置合理 bounds，并决定 missed deadline 的处理策略。`dt <= 0`、NaN 和巨大 gap 都不能静默进入公式。

## 4. PID、saturation 与 anti-windup

parallel-form PID：

```text
u = Kp * e + Ki * integral(e dt) + Kd * de/dt
```

actuator 只能输出有限范围。若 command 已饱和但 integral 继续积累，error 反向后 controller 仍可能长时间卡在饱和，
这就是 integrator windup。常见策略包括：

- conditional integration：饱和且 error 继续推向饱和方向时停止积分；
- integral clamping：直接限制 integral state；
- back-calculation：用 saturated 与 unsaturated output 的差修正 integral。

策略和 tuning 必须匹配。练习采用 conditional integration，是为了清楚观察 state，不代表所有 plant 的最佳方案。

bumpless transfer 也很重要：manual → automatic 或 controller 切换时，应初始化 internal state，避免 command jump。

## 5. Filtering、hysteresis 与 alarms

一阶离散 low-pass filter：

```text
y[k] = y[k-1] + alpha * (x[k] - y[k-1]),  0 <= alpha <= 1
```

较小 `alpha` 更平滑但延迟更大。filter 会改变 phase/dynamics，不能为了曲线好看随意叠加。first sample 如何初始化、
reset 后如何工作、invalid measurement 如何处理，都属于接口 contract。

hysteresis 使用不同的 ON/OFF thresholds。例如 signal 上升到 high 才打开，下降到 low 才关闭。它减少 noisy
threshold 附近的 chatter。deadband、debounce 和 time qualification 是相关但不同的工具：一个针对幅度区间，一个
针对持续时间。

## 6. State machine、interlock 与 fail-safe

工业 sequence 应显式表示 state、transition、guard、timeout 和 fault。不要把安全逻辑散落成互相覆盖的 booleans。

interlock 是允许 command 执行的必要条件；alarm 是通知，不一定阻止动作；trip 通常触发保护动作。真实系统中的
priority、latching、reset authority 和 redundancy 由 hazard analysis 与行业标准决定，不能凭软件便利性自行猜测。

## 7. C++ implementation concerns

- controller hot path 避免不可控 allocation、blocking I/O 和 exceptions；
- configuration 在启用前验证，使用 strong types/units 避免混淆 Hz、seconds、milliseconds、volts；
- 明确 floating-point NaN/Inf policy，不让 comparison 静默失效；
- state 由单一 execution context 拥有，或清楚同步，避免 data race；
- telemetry/logging 不应阻塞 control loop，可异步传递 snapshot；
- tests 使用 deterministic sample sequences，不依赖 wall-clock sleep；
- 保存 input、configuration、timestamp 和 output，支持 replay/incident analysis。

## 8. 电力行业复习边界

下一阶段可学习 per-unit system、three-phase power、frequency/voltage regulation、power factor、transformer/CT/PT、
breaker、relay、IEC 61850/DNP3/SCADA 基础。但应分别标注“理论复习”“protocol/domain knowledge”和“实际工作经验”，
不能因为写过 PID exercise 就声称做过 grid protection 或 power-system control。

## 9. Senior interview checklist

解释一个 control implementation 时依次回答：

1. Plant、controlled variable、setpoint、measurement 和 actuator 是什么？
2. Sample period、latency、jitter 与 missed deadline policy 是什么？
3. Units、limits、rate limit、saturation 和 anti-windup 如何定义？
4. Noise、filter delay、hysteresis 与 alarm semantics 如何处理？
5. Invalid/stale data、restart、mode transition 和 communication loss 时系统做什么？
6. 如何用 simulation、recorded traces、boundary tests 和 hardware-in-the-loop 验证？
