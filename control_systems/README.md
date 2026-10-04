# Control Systems — Power/Industrial C++ Track

这是岗位定向复习模块：重新激活本科控制论知识，并把它转换成能在工业自动化、电力软件和 Senior C++
面试中讨论、实现、测试的工程能力。它不是在简历中虚构电力控制经验。

固定顺序：

1. **Concept**：阅读 [`CONTROL_SYSTEMS.md`](CONTROL_SYSTEMS.md)。
2. **Quiz**：完成 [`EXERCISES.md`](EXERCISES.md)，答案不进入 `main`。
3. **Coding**：完成 PID、low-pass filter 和 hysteresis TODO。
4. **Tests**：验证 nominal、boundary、saturation 和 invalid-input behavior。
5. **Interview explanation**：说明 sample time、units、stability、failure mode 和 operational safety。

## 第一阶段范围

- feedback loop、setpoint、measurement、error、actuator；
- continuous model 到 discrete implementation；
- P/PI/PID、output saturation、integrator windup、derivative noise；
- first-order low-pass filter 与 sensor noise；
- hysteresis/deadband 防止 threshold chatter；
- state machine、interlock、manual/automatic mode、fail-safe behavior；
- deterministic timing、timestamp/data quality、logging 与 replay testing；
- SCADA supervisory control、local controller 与 protection relay 的职责边界。

## Coding exercises

1. `pid_controller`：固定 `dt` 的 PID、output clamp 和 conditional-integration anti-windup。
2. `low_pass_filter`：离散一阶滤波、first-sample initialization 和 reset。
3. `hysteresis_switch`：带上下阈值的稳定开关，避免 noisy signal 反复切换。

```sh
cmake -S . -B build
cmake --build build --target pid_controller_exercise_tests low_pass_filter_exercise_tests hysteresis_exercise_tests
ctest --test-dir build -R control_ --output-on-failure
```

在 `solution/control-systems` 完成答案，review 后再合并到 `answers`；不要 merge 回 `main`。
