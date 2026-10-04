# Database — Industrial C++ 80/20

目标是掌握应用开发中最常用、最容易影响正确性的部分，而不是学习数据库内核。总时间盒：半天到一天，最多
4 个 90-minute sessions。

1. 阅读 [`DATABASE_CORE.md`](DATABASE_CORE.md)。
2. 完成 [`EXERCISES.md`](EXERCISES.md) 的 SQL、判断与设计题。
3. 在 [`exercises/sql_queries`](exercises/sql_queries) 写查询，不把答案提交到 `main`。
4. 完成 `transaction_guard` TODO，用 fake connection 验证 commit/rollback，不依赖外部数据库。

```sh
cmake -S . -B build
cmake --build build --target transaction_guard_exercise_tests
ctest --test-dir build -R database_ --output-on-failure
```

答案放在 `solution/database`，review 后合并到 `answers`。

## 时间盒

| Session | 内容 |
|---|---|
| 1 | SELECT/WHERE/JOIN/GROUP BY/NULL + SQL trace |
| 2 | schema、constraints、indexes、query-plan reasoning |
| 3 | transactions、isolation、idempotency、C++ RAII exercise |
| 4 | telemetry/alarm scenario + quiz + 60 秒解释 |
