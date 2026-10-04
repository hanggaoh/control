# Database Exercises

答案只保存在 `solution/database` 和 `answers`，不进入 `main`。

## A. 选择与判断

1. **判断**：`column = NULL` 可以找到 NULL rows。
2. **单选**：查询 `device_id = ?` 且按时间范围读取，最值得首先评估的 composite index 是：
   A. `(measured_at, value)`；B. `(device_id, measured_at)`；C. `(value)`；D. 每列单独一个 index。
3. **判断**：使用 transaction 后，任何异常都可以安全地无限 retry。
4. **单选**：防止 SQL injection 的首要应用措施是：A. 手工 escape；B. parameterized statement；
   C. 删除空格；D. 使用更长的 SQL string。
5. **判断**：connection pool 中取出的同一个 connection 可以默认由多个线程同时使用。
6. **单选**：UI 频繁显示每个设备当前值，历史表非常大。最合理的第一步是：A. 每次 full scan；
   B. 定义 latest-state read pattern 并验证一致性；C. 删除历史；D. 在 UI 中缓存 raw pointer。

## B. Senior 简答

1. 解释 transaction RAII guard 为什么 destructor 应 rollback 而不是 commit。
2. 比较 optimistic version check 与 pessimistic locking 的适用场景。
3. 为什么 retry 一个 outcome unknown 的 command 可能产生 duplicate side effect？如何使用 idempotency key？
4. 设计 telemetry row：哪些 timestamp、quality、identity 和 value metadata 必须明确？
5. network outage 后 store-and-forward 如何处理 ordering、duplicates、capacity 和 late data？
6. query 很慢时，你会按什么顺序检查 result grain、row count、query plan、index 和 access pattern？

## C. SQL practice

使用 `exercises/sql_queries/schema.sql`，在 `queries.todo.sql` 完成：

1. 查询指定设备时间范围内的 measurements；
2. 统计每个设备一小时内的 sample count 和 average value；
3. 查询尚未 cleared 的 alarms，并包含 device name；
4. 找出没有任何 telemetry 的 devices；
5. 设计“每台设备最新 measurement”的查询，并说明 index。
