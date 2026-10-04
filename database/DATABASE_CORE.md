# Database Core — 80/20

## Tier 1 — 必须掌握

### SQL query shape

必须能写和读：`SELECT`、`WHERE`、`INNER/LEFT JOIN`、`GROUP BY`、`HAVING`、`ORDER BY`、`LIMIT`，以及基本
aggregate：`COUNT`、`MIN/MAX`、`AVG`。先确认 result grain：一行代表 device、alarm 还是 measurement，避免 join
后意外重复计数。

`NULL` 表示 unknown/missing，不等于零或空字符串。使用 `IS NULL`，并理解 SQL three-valued logic。`LEFT JOIN`
右侧 column 的 filter 放在 `WHERE` 可能把结果悄悄变成 inner-join behavior。

### Schema and constraints

- primary key 唯一标识 row；
- foreign key 表达 relationship 和 referential integrity；
- `NOT NULL`、`UNIQUE`、`CHECK` 尽量让数据库保护 invariant；
- normalized operational schema 减少更新异常，但 read model/report 可合理 denormalize；
- timestamp 明确 UTC/offset、precision 和 clock source；
- telemetry 同时保存 value、unit/type、quality、source timestamp 和 ingestion timestamp。

### Indexes

index 用额外 storage 和 write cost 换 read speed。优先围绕实际 query pattern 设计，而不是给每列加 index。
例如 `WHERE device_id = ? AND measured_at >= ? ORDER BY measured_at` 通常先评估
`(device_id, measured_at)`，再通过 query plan 和实际 workload 验证。

### Transactions and ACID

transaction 把多个变更定义为一个 atomic unit。应用层至少要理解 atomicity、consistency、isolation、durability，
以及 dirty read、non-repeatable read、phantom/lost update。不要盲目使用最高 isolation；根据 invariant、contention
和数据库能力选择，并处理 serialization/deadlock failure。

### Safe C++ boundary

- connection、statement、result、transaction 用 RAII 管理；
- 使用 parameterized/prepared query，不拼接用户输入；
- transaction scope 短，不在其中做 network call 或等待 UI；
- pool 中的 connection 不默认 thread-safe，一次 checkout 由一个 execution context 使用；
- error 分类为 transient、constraint/business、permanent；不是所有 failure 都 retry；
- retry 必须考虑 idempotency、outcome unknown 和 duplicate command。

## Industrial/power data considerations

- latest state 与 immutable telemetry history 是不同 access patterns；
- telemetry 保存 source/ingestion timestamp、quality、sequence 和 unit/type；
- alarm 的 occurrence、acknowledgement、clear 应分别记录；
- store-and-forward 要定义 bounded capacity、overflow、deduplication、ordering 和 retry；
- retention、archive/downsampling 和 time partition 防止 history 无界增长。

## Tier 2 — 知道何时使用

- optimistic concurrency/version column；
- upsert 与 idempotency key；
- window functions，例如 per-device latest row；
- migrations 和 backward-compatible rollout；
- read replica、eventual consistency、outbox pattern；
- time-series database 与 relational database 的选择。

## 笔试前明确排除

- B-tree/WAL/LSM 内核实现细节；
- 分布式 consensus 算法证明；
- vendor-specific replication administration；
- 深度 query optimizer internals；
- 大规模 data warehouse/OLAP tuning。

Senior 回答顺序：先说 data invariant 和 result grain，再说 schema/transaction，然后说 index/performance，最后补
failure、retry、observability 和 migration。
