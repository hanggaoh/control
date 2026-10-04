# Copy-and-swap TODO exercise

实现 `copy_swap_buffer`，目标：

- destructor 释放资源；copy constructor 做 deep copy；
- member/non-member `swap` 均为 `noexcept`；
- copy assignment 使用 copy-and-swap，并安全处理 self-assignment；
- 测试注入 copy failure 时，assignment target 保持原值且没有 leak。

只修改 `include/copy_swap_buffer.hpp` 的 TODO。完成后把 `exercise_complete` 改为 `true`：

```sh
cmake --build build --target copy_swap_exercise_tests
ctest --test-dir build -R foundation_copy_swap_exercise --output-on-failure
```

思考：生产代码中若 representation 可以直接使用 `std::vector<int>`，为什么 Rule of Zero 更好？
