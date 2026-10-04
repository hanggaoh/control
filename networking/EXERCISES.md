# Networking Quiz

答案只放在 `solution/networking`。

## 选择/判断

1. 判断：一次 TCP `send` 成功后，对端一次 `recv` 必定得到相同边界的数据。
2. 单选：收到 2-byte length header 时应：A. 当 malformed；B. 等待更多 bytes；C. 补零；D. reconnect。
3. 判断：先按 peer length 分配 memory，再检查 maximum frame size 是安全的。
4. 单选：partial write 后应：A. 丢弃剩余数据；B. 从未发送 offset 继续；C. 重发整个 stream；D. 关闭日志。

## Senior 简答

1. 设计 incremental frame parser 的 invariant。
2. timeout、disconnect、protocol error 的 retry policy 为什么不应完全相同？
3. 如何防止旧 connection 的 late callback 污染新 session？
4. telemetry sequence number 与 timestamp 分别解决什么问题？
