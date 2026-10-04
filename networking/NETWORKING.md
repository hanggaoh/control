# TCP and Protocol Essentials

TCP 提供可靠、有序的 byte stream，但不保留 application message boundaries。一次 `send` 不对应一次 `receive`；一次
read 可能只得到 header 的一部分，也可能同时得到多个 frames。

本练习使用 4-byte big-endian payload length：

```text
+----------------------+-------------------+
| uint32 payload bytes | payload           |
+----------------------+-------------------+
```

incremental decoder 应反复执行：缓存新 bytes；不足 4 bytes 就等待；读取并验证 length；不足完整 payload 就等待；
完整时产出 frame 并继续解析剩余 bytes。必须在 allocation 前验证 maximum length，避免 hostile length 导致 unbounded
allocation。parse error 后应进入明确 error state，不能悄悄失去 framing synchronization。

Production concerns：partial writes、write queue ordering、read/write timeout、peer close、retry/backoff、heartbeat、
versioning、endianness、integer overflow、telemetry freshness 和 observability。
