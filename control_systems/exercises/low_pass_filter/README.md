# First-order low-pass filter TODO exercise

补全 `update`：first sample 直接初始化 output，之后使用
`output += alpha * (input - output)`。`reset` 后下一次输入重新作为 first sample。

完成后将 `exercise_complete` 改为 `true`：

```sh
cmake --build build --target low_pass_filter_exercise_tests
ctest --test-dir build -R control_low_pass_filter --output-on-failure
```
