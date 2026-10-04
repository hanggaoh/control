# Top-K TODO exercise

补全 `top_k_largest`：使用大小最多为 `k` 的 min-heap，返回 descending results。`k == 0` 返回空；
`k > input.size()` 时返回所有元素。目标复杂度为 `O(n log k)` time、`O(k)` extra space。

完成后将 `exercise_complete` 改为 `true`：

```sh
cmake --build build --target top_k_exercise_tests
ctest --test-dir build -R algorithm_top_k --output-on-failure
```
