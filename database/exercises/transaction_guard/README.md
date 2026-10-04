# Transaction guard TODO exercise

实现 move-only RAII transaction：constructor 已调用 `begin()`；`commit()` 最多执行一次；未 commit 的 active transaction
在 destructor 中 rollback。move 后 source 必须 inactive，不能产生双 rollback。

这里只用 fake connection 验证 lifetime contract，不绑定具体 database library。

```sh
cmake --build build --target transaction_guard_exercise_tests
ctest --test-dir build -R database_transaction_guard --output-on-failure
```
