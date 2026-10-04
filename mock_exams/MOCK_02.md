# Mock 02 — Async, Windows UI, and Control (90 minutes)

## Part A — Boost.Asio lifecycle (25 points, 20 minutes)

画出 connect → read → timeout → cancel → reconnect 流程。解释 `shared_from_this`、strand、generation token 和
`operation_aborted`，并给出 graceful stop 顺序。

## Part B — MFC modernization (20 points, 20 minutes)

一个 legacy `CDialog` 同时做 socket I/O、parsing、control calculation 和 rendering。提出增量重构步骤、interfaces、thread
boundary 与 characterization tests，不允许 big-bang rewrite。

## Part C — Control correctness (25 points, 20 minutes)

解释 PID saturation/integrator windup；设计 anti-windup；说明 low-pass filter 与 hysteresis 的不同作用；列出 stale/NaN
measurement policy 中需要 domain owner 决定的部分。

## Part D — Integrated design (30 points, 30 minutes)

设计 operator station：TCP telemetry、validated command、MFC UI、local simulator。给 component diagram、ownership、thread
model、bounded queue policy、observability 和 safe shutdown。指出一个可能的 use-after-free 和一个 overload failure。
