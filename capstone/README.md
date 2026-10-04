# Final Capstone — Industrial Control Station

最终项目将 Boost.Asio 与 MFC 当作 integration boundaries，把前面的 C++ 基础、算法、控制逻辑、并发、网络和
可靠性练习合成一个系统，而不是做两个互不相关的 framework demo。

项目场景：Windows operator station 通过 TCP 连接一个 simulated industrial/power device，接收 telemetry，显示
状态和 alarms，发送经过 validation/interlock 的 supervisory commands，并能处理 timeout、disconnect、reconnect
和 graceful shutdown。

## 蓝图文档

- [`SYSTEM_DESIGN.md`](SYSTEM_DESIGN.md)：系统设计概念、接口、ownership、故障与取舍。
- [`SYSTEM_DESIGN_TODO.md`](SYSTEM_DESIGN_TODO.md)：90 分钟纸笔设计练习与四项验收标准。
- [`ARCHITECTURE.md`](ARCHITECTURE.md)：组件边界、dependency direction、thread/lifetime model。
- [`MILESTONES.md`](MILESTONES.md)：从 pure C++ core 到 Boost.Asio、MFC 和 resilience 的实施顺序。
- [`ACCEPTANCE_CRITERIA.md`](ACCEPTANCE_CRITERIA.md)：功能、可靠性、测试和面试演示标准。

## 目标分支

```text
main
└── solution/capstone-control-station
        └── answers
```

`main` 只保存蓝图、interfaces、TODO 和 tests。实现放在 `solution/capstone-control-station`，review 后合并到
`answers`，不反向合并到 `main`。

## 最终贯穿关系

| 已学模块 | Capstone 中的落点 |
|---|---|
| Foundations | RAII、ownership、lifetime、exception guarantees、`shared_from_this` |
| Algorithms/STL | receive buffer、lookup table、timer queue、bounded history、invalidation |
| Control systems | filtering、hysteresis、PID simulation、interlock、mode/state machine |
| `unique_handle` | Windows/socket/native resource ownership |
| bounded queue | I/O/control/UI 之间的 bounded message transfer |
| TCP framing | partial reads、length validation、message decoding |
| Boost.Asio | async connect/read/write、strand、timer、cancellation、reconnect |
| MFC | thin UI adapter、UI-thread affinity、message posting、view model rendering |
