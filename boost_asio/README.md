# Boost.Asio Lifecycle Practice

当前环境未发现 Boost headers，因此本模块不会安装依赖或让主构建失败。第一阶段用 portable lifecycle model 练习
真正困难的部分：outstanding handlers、generation token、reconnect、stop 和 late completion。真实 Asio adapter 在
`solution/boost-asio` 且依赖可用时实现。

顺序：[`BOOST_ASIO.md`](BOOST_ASIO.md) → [`EXERCISES.md`](EXERCISES.md) → lifecycle TODO → tests。

系统设计关联：阅读 [`Capstone system design`](../capstone/SYSTEM_DESIGN.md)，再完成
[`90-minute design TODO`](../capstone/SYSTEM_DESIGN_TODO.md) 中的 session ownership、timeout、backpressure 和 shutdown 部分。

```sh
cmake --build build --target asio_lifecycle_exercise_tests
ctest --test-dir build -R asio_ --output-on-failure
```
