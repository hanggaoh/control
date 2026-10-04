# Mock 01 — Core C++ and Networking (90 minutes)

## Part A — Concepts (20 points, 20 minutes)

1. 解释 throwing move 如何影响 `vector` relocation。（5）
2. 找出返回 temporary `const&` 的 lifetime bug，并给安全 API。（5）
3. 比较 `map`、`unordered_map`、sorted `vector` 的 lookup/update trade-off。（5）
4. 解释两个 `shared_ptr(raw)` 为什么可能 double delete。（5）

## Part B — Code reasoning (30 points, 25 minutes)

1. 修复一个 self-move-unsafe resource assignment，并说明 invariant。（10）
2. 给出 condition-variable bounded queue 的 wait predicates 和 close semantics。（10）
3. 写出 lower-bound pseudocode，覆盖 empty/duplicate/not-found。（10）

## Part C — Coding (30 points, 30 minutes)

实现 length-prefixed incremental decoder 的核心 loop。要求处理 fragmented header/payload、multiple frames、maximum
length 和 malformed state。可写 compilable C++ 或精确 pseudocode。

## Part D — Scenario (20 points, 15 minutes)

客户端收到 telemetry 一段时间后连接断开。设计 timeout、reconnect/backoff、duplicate/stale message handling、logging 和
shutdown。明确哪些 state 由哪个 thread/context 拥有。
