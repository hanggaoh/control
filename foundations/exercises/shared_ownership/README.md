# Shared ownership and async lifetime TODO exercise

完成两个不会执行 UB 的安全改写：

1. `share_existing` 必须复制已有 `shared_ptr`，复用 control block；不要从 `existing.get()` 重建 owner。
2. `async_session::make_callback` 必须捕获 `shared_from_this()`，让 callback 持有既有 ownership，不能捕获
   bare `this`，也不能写 `shared_ptr<async_session>(this)`。

测试用 `owner_before` 检查 ownership identity，并验证外部 owner reset 后 callback 仍可安全执行。它不会构造
两个独立 control blocks，因为那会把 double deletion 带进测试本身。

完成 TODO 后把 `exercise_complete` 改为 `true`：

```sh
cmake --build build --target shared_ownership_exercise_tests
ctest --test-dir build -R foundation_shared_ownership_exercise --output-on-failure
```
