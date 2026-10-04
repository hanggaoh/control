# Binary-search boundary TODO exercise

补全 `first_not_less`，返回 sorted input 中第一个 `>= target` 的 index；若不存在则返回 `values.size()`。
要求使用 half-open range，并避免 midpoint overflow。

完成后将 `exercise_complete` 改为 `true`：

```sh
cmake --build build --target binary_search_exercise_tests
ctest --test-dir build -R algorithm_binary_search --output-on-failure
```
