# Boost.Asio Quiz

答案只放在 `solution/boost-asio`。

1. 判断：`timer.cancel()` 后 completion handler 一定不会运行。
2. 判断：捕获裸 `this` 的 handler 在 socket open 时一定安全。
3. strand 解决什么问题？它不解决什么问题？
4. 为什么 session generation 可以阻止旧连接 callback 修改新连接？
5. 写出 window close 到 `io_context` thread joined 的 shutdown 顺序。
