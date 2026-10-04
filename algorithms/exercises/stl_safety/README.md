# STL safety TODO exercise

补全 `erase_negative`，删除所有负数并保留其余元素的相对顺序。使用 C++20 STL algorithm，不要手写一个在
`erase` 后继续递增失效 iterator 的 loop。

完成后将 `exercise_complete` 改为 `true`：

```sh
cmake --build build --target stl_safety_exercise_tests
ctest --test-dir build -R algorithm_stl_safety --output-on-failure
```
