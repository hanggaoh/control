# Self-move-safe unique resource TODO exercise

补全 `tracked_resource::operator=(tracked_resource&&)`：正常 move assignment 要先释放 destination 的旧资源并
transfer ownership；`x = std::move(x)` 又必须保留原资源，且任何路径都不能 double release。

只修改 `include/unique_resource.hpp` 的 TODO。完成后把 `exercise_complete` 改为 `true`：

```sh
cmake --build build --target unique_resource_exercise_tests
ctest --test-dir build -R foundation_unique_resource_exercise --output-on-failure
```

面试解释：`this` 和 `&other` alias 时，release-before-transfer 的每一步分别读写了哪个对象？
