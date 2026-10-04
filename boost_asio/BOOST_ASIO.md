# Boost.Asio Mental Model

Asio async initiation function 通常立即返回；completion handler 稍后在关联 executor 上执行。必须同时推理 object
lifetime、operation lifetime 和 executor serialization。

核心规则：每个 socket 保持 single read chain；writes 经 queue 串行；timer cancellation 仍可能产生带
`operation_aborted` 的 completion；`shared_from_this` 可让 session 活到 handler 完成；stop 后不能让 late handler 再次
schedule reconnect。generation token 能让旧 session completion 被识别为 stale。

strand 保证经它调度的 handlers 不并发执行，但不自动保护绕过 strand 的访问，也不负责 object lifetime。graceful
shutdown 应停止新工作、cancel timers/socket、让 completions 收敛、停止 context 并 join threads。
