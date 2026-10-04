# Discrete PID with anti-windup TODO exercise

补全固定 sample time PID：计算 P/I/D，输出 clamp 到 limits；若已饱和且当前 error 会继续推向同一饱和方向，
不要提交 candidate integral。处理 first update，保持 `update` hot path 无 allocation、无 blocking、`noexcept`。

练习公式定义在 `pid_controller.hpp`。完成后将 `exercise_complete` 改为 `true`：

```sh
cmake --build build --target pid_controller_exercise_tests
ctest --test-dir build -R control_pid_controller --output-on-failure
```

这只验证实现 contract，不证明任何真实 plant 的 closed-loop stability 或 tuning 正确。
