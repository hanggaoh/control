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

## Rule of Zero 对照实践

[rule_of_zero_buffer.hpp](include/rule_of_zero_buffer.hpp) 保持相同的 size/index 接口，
但用 vector 替代 raw pointer 和独立 size。Vector 管理 storage、deep copy、move 和 destruction；
编译器生成的 special member functions 调用它的对应操作，所以这个类不手写资源管理代码。

先用自己的话解释：为何复制 wrapper 仍得到独立内容？为何不再需要维护 size 与 buffer 的一致性？
然后阅读 [测试](tests/rule_of_zero_tests.cpp)，运行两组测试：

```sh
cmake -S . -B build
cmake --build build --target copy_swap_exercise_tests rule_of_zero_exercise_tests
ctest --test-dir build -R 'foundation_(copy_swap|rule_of_zero)_exercise' --output-on-failure
```

进一步实践：为两个 buffer 增加 checked access 的接口，先定义越界行为，再增加对应测试；
这里不提供实现。比较它与 operator[] 的调用方 contract。

Rule of Zero 不表示自动实现了 copy-and-swap，也不自动给任意多成员类提供 strong guarantee。
此对照类没有故障注入开关；原练习仍用于观察 prepare/commit/cleanup 的异常路径。

> I delegate storage ownership and value semantics to vector, so the wrapper follows the Rule of Zero.
