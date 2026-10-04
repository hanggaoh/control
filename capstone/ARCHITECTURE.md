# Architecture Blueprint

## 1. Dependency direction

```text
MFC UI adapter ───────────────┐
                             v
                     Application services
                             |
              ┌──────────────┼──────────────┐
              v              v              v
        Domain model   Control policies   Protocol model
              ^                             ^
              |                             |
Boost.Asio transport adapter ───────────────┘
              ^
              |
      Device simulator adapter
```

核心规则：domain/application code 不 include MFC 或 Boost.Asio headers。framework-specific types 停留在 adapters，
通过小型 pure C++ interfaces 进入 core。这样 control、protocol 和 state transitions 可以在 Linux/CI 或普通 console
test 中验证，MFC 只负责 Windows presentation。

## 2. Proposed components

### Domain and application core

- `DeviceId`, `Timestamp`, strongly typed measurements/units；
- `ConnectionState`, `OperatingMode`, `AlarmState`；
- `TelemetrySnapshot`：immutable UI/read-model snapshot；
- `CommandRequest` 与 validation result；
- `InterlockPolicy`：决定 command 是否允许；
- `ControlPipeline`：filter → hysteresis/PID simulation → limit checking；
- `StationService`：协调 connection events、telemetry、commands 与 shutdown。

### Protocol

- length-prefixed binary frame；
- message type、version、sequence number、payload length；
- maximum-frame limit、unknown-version rejection、malformed-input handling；
- incremental parser：支持 partial header、partial payload 和 multiple frames per read；
- encoder 与 parser 都不依赖 socket。

### Boost.Asio adapter

- `io_context` ownership 和 worker thread lifetime 明确；
- session 通过 `enable_shared_from_this` 管理 outstanding handlers；
- single read chain；write queue 保证 frame 不交叉；
- strand 或 single executor 保证 session state serialized；
- connect/read/heartbeat timers；
- cancellation 与 generation/session token 防止 stale completion 修改新连接；
- bounded exponential backoff + jitter；
- error_code 被翻译成 application event，不泄漏到 domain API。

### MFC adapter

- `CDialog`/`CView` 不包含 protocol parser、socket state machine 或 control algorithm；
- UI command 转换为 `CommandRequest`，交给 `StationService`；
- background thread 不直接访问 MFC controls；
- 使用 `PostMessage` 或等价 dispatcher 将 immutable snapshot 交回 UI thread；
- window destruction 时先断开 callbacks/dispatcher，再停止后台 services；
- UI 显示 connection、data quality、timestamp、alarm 与 command rejection reason。

### Simulator

- 和真实 transport 使用同一 protocol；
- 可生成 normal/noisy/stale/out-of-range telemetry；
- 可注入 fragmentation、delay、disconnect、malformed frame；
- simple plant model 用于 control exercise，不宣称模拟真实电网动态；
- seeded scenario 保证 tests/replay deterministic。

## 3. Thread ownership model

建议先用三个 execution contexts：

| Context | Owns | Must not do |
|---|---|---|
| UI thread | MFC windows/controls、rendered view model | blocking network I/O、control-loop work |
| Asio thread | socket、timers、session state、write queue | access MFC controls |
| control worker | control state、filter/PID/interlock evaluation | mutate socket or UI objects directly |

跨 boundary 传递 value/immutable snapshot，queue 必须 bounded，并定义 overload policy：drop/coalesce telemetry、保留 critical
state transition，commands 不得静默丢失。不要用一个 global mutex 把所有 component 粘起来。

## 4. Lifetime and shutdown sequence

```text
UI requests close
  → reject new commands
  → detach UI callback/dispatcher
  → request StationService stop
  → cancel reconnect/heartbeat/read/write
  → close socket and drain completion handlers
  → close worker queues and join control worker
  → stop io_context and join Asio thread
  → destroy services
  → destroy MFC window
```

shutdown 必须 idempotent。late completion 只能观察 stopped/generation state 后退出，不能重新 schedule reconnect。
destructor 是最后防线，不应依靠 destructor 在 UI thread 上执行长时间 blocking shutdown。

## 5. Command safety path

```text
MFC input
  → parse/type conversion
  → authentication/authorization boundary (documented or simulated)
  → operating-mode check
  → range and rate-limit validation
  → data-quality/freshness check
  → interlock policy
  → protocol encode
  → ordered async write
  → acknowledgement/timeout
  → audit event + UI result
```

哪些 command 应被拒绝、哪些 interlock 可 bypass、谁有 reset authority，必须来自 domain/safety requirements；代码练习
只实现明确给定的 policy，不能自行发明真实电力保护规则。

## 6. Observability

每个 connection attempt、state transition、command、rejection、timeout 和 parse error 都带 correlation/session ID 与 monotonic
timestamp。metrics 至少包含 reconnect count、parse failures、queue high-water mark、telemetry age 和 command latency。
logging 异步化且 bounded，不能阻塞 control/network hot path。
