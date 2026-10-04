# Async session lifecycle TODO

补全 state transitions。每个 start/reconnect generation 唯一；只有 matching generation 的 completion 能改变 state；
stop 会使所有旧 completions stale，并禁止 reconnect。

这是真实 Asio handler lifecycle 的 deterministic model，不是 Asio API 替代品。
