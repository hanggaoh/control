# Hysteresis switch TODO exercise

补全双阈值 switch：OFF 时只有 `input >= high` 才切到 ON；ON 时只有 `input <= low` 才切到 OFF；阈值之间保持
当前状态。假设构造前已验证 `low < high`。

完成后将 `exercise_complete` 改为 `true`：

```sh
cmake --build build --target hysteresis_exercise_tests
ctest --test-dir build -R control_hysteresis --output-on-failure
```
