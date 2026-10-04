# `noexcept` move and reference lifetime TODO exercise

两个小任务：

1. 为 `relocation_probe` 补全 copy/move constructor，并观察 `vector` reallocation 使用哪一个。
2. 补全 `relocation_source`，用 `std::move_if_noexcept` 观察 throwing move + available copy 时的选择。
3. 补全 `stable_name_view`，让返回的 `string_view` 在 owner 活着且未修改时有效；解释为什么
   `const std::string& make_name(){ return std::string("sensor"); }` 不会把 temporary lifetime 延长到 caller。

完成 TODO 后把 `exercise_complete` 改为 `true`，运行：

```sh
cmake --build build --target move_lifetime_exercise_tests
ctest --test-dir build -R foundation_move_lifetime_exercise --output-on-failure
```

额外实验：暂时移除 move constructor 的 `noexcept`，重新运行并比较 copy/move counters（不要把实验改动提交为答案）。
