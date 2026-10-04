# Capstone Milestones

每个 milestone 都应能独立构建、测试和讲解。不要先创建一个巨大 MFC/Asio class 再回头拆分。

## M0 — Contracts and pure models

- 定义 typed domain values、states、events 和 interfaces；
- 写 architecture decision notes：threading、ownership、error policy；
- tests 覆盖 state transition 与 invalid configuration。

Exit：无 Boost/MFC dependency，core tests 可运行。

## M1 — Protocol and incremental parser

- frame header、encoder、incremental decoder；
- partial/multiple/malformed/oversized input tests；
- fuzz/property-style invariant：parser 不越界、不无限增长。

连接：STL storage、span/view lifetime、binary parsing、exception safety。

## M2 — Domain state and control pipeline

- telemetry validation、freshness/data-quality；
- low-pass/hysteresis/PID simulator；
- interlock、manual/automatic mode、alarm state machine；
- recorded deterministic scenarios。

连接：control systems、algorithms、state machine、numeric edge cases。

## M3 — Boost.Asio transport

- async connect/read/write；
- strand/single-executor serialization；
- heartbeat、timeout、cancellation、reconnect/backoff；
- fake/local simulator integration tests。

Exit：反复 connect/disconnect 不 leak、不 overlap read、不 duplicate reconnect。

## M4 — Device simulator

- normal telemetry 与 command acknowledgement；
- fragmentation、delay、disconnect、corruption injection；
- repeatable scenario seeds 和 replay files。

Exit：可从 command line 演示 recovery，无 MFC dependency。

## M5 — MFC thin UI

- connection/status/telemetry/alarm views；
- operator command form 与 rejection feedback；
- background → UI thread dispatcher；
- close window 时执行 safe shutdown。

Exit：UI classes 无 protocol/control implementation；worker threads 不直接访问 controls。

## M6 — Reliability hardening

- bounded queues 与 overload policy；
- stale callback/session generation protection；
- telemetry coalescing、audit trail、metrics；
- fault-injection soak test、rapid open/close test、clean process exit。

## M7 — Senior interview package

- architecture diagram 与 5-minute walkthrough；
- 一个 lifetime bug、一个 framing bug、一个 shutdown bug 的复盘；
- trade-offs：strand vs mutex、strong vs weak callback capture、queue drop policy；
- 明确区分理论复习、simulator experience 和真实 production/domain experience。
