# Algorithms and Data Structures — 80/20 Core

目标不是覆盖所有经典题型，而是用最少内容解决大多数 C++ 笔试和工程判断题。优先级由使用频率、迁移能力、
与当前岗位的相关性共同决定。

## Tier 1 — 必须熟练

### 1. `vector` + linear scan

覆盖：默认 sequence、contiguous storage、cache locality、index/iterator、erase、reallocation invalidation。

必须会：

- 单次遍历统计、过滤、转换；
- `reserve` 与 capacity；
- `erase_if` / erase-remove；
- append/erase 后 pointer、reference、iterator 是否有效；
- 为什么实际工程中 `vector` 通常优先于 `list`。

典型场景：TCP receive buffer、telemetry batch、bounded history、lookup table。

### 2. `unordered_map` / `unordered_set`

覆盖：lookup、counting、deduplication、indexing、grouping。

必须会：

- frequency map、seen set、key → state/session；
- `find`、`contains`、`try_emplace`；
- average `O(1)` 与 worst-case degradation；
- rehash、`reserve`、iterator invalidation；
- 不要用 `operator[]` 做只读查询并意外插入。

典型场景：device/session lookup、message deduplication、alarm aggregation、LRU index。

### 3. sorting + binary search

覆盖：先排序再扫描、ordered lookup、boundary search。

必须会：

- `sort` comparator 必须满足 strict weak ordering；
- `lower_bound` / `upper_bound` 的语义；
- half-open range `[first, last)`；
- overflow-safe midpoint；
- sort `O(n log n)` 与 repeated linear lookup 的取舍。

典型场景：configuration lookup、time-range query、event ordering、duplicate grouping。

### 4. queue/deque + BFS/state processing

覆盖：FIFO、level/order processing、producer-consumer mental model。

必须会：

- `queue` 通常由 `deque` 支撑；
- BFS 的 visited 时机：通常 enqueue 时标记，避免重复入队；
- bounded queue 的 full/empty/closed states；
- queue growth 必须有 capacity/backpressure policy。

典型场景：work queue、event processing、dependency traversal、connection state events。

### 5. heap / `priority_queue`

覆盖：top-K、next deadline、priority scheduling。

必须会：

- min-heap/max-heap 的选择；
- push/pop `O(log n)`，top `O(1)`；
- size-k min-heap 求 largest K；
- priority queue 不适合 arbitrary removal；
- stale timer entry 可通过 generation/version 做 lazy discard。

典型场景：retry deadlines、timer scheduling、top-K telemetry/anomalies。

## Tier 2 — 会识别并写基础版本

### 6. Two pointers / sliding window

适用于 contiguous input 上的 subrange 问题：去重、partition、固定/可变窗口、最近 N 个 samples。必须能说明
window invariant，以及 left/right 每次移动为何保持 invariant。若数据包含 negative values，不要盲目套用依赖
monotonic sum 的窗口模板。

### 7. Stack

用于 nested structure、括号匹配、expression/parser state、DFS iterative form。当前岗位最有价值的是理解 parser 和
explicit state，不需要刷大量 monotonic-stack 技巧题。

### 8. DFS/BFS + cycle detection

掌握 adjacency list、visited state、connected traversal 和三色 DFS cycle detection。应用到 service dependencies、
startup/shutdown ordering，而不是深入所有图论算法。

### 9. Prefix sum / running aggregate

掌握 repeated range-sum 与 cumulative metrics。知道它用 `O(n)` preprocessing 换 `O(1)` range query，同时注意
integer overflow 和 dynamic-update limitation。

## Tier 3 — 只需知道何时使用

- `map` / `set`：需要 ordered keys、range query 或稳定的 logarithmic behavior；
- `list`：需要 stable iterators 和 splice，且已经确认 cache/allocation cost 可接受；
- tree：理解 BST/heap/trie 的用途，不手写通用 production container；
- Union-Find：dynamic connectivity；
- basic dynamic programming：能识别 overlapping subproblems，但笔试前不刷复杂状态设计。

## 笔试前明确排除

- advanced DP、bitmask DP；
- segment tree、Fenwick tree；
- suffix array/automaton；
- advanced shortest path / max flow；
- red-black tree 手写实现；
- lock-free queue；
- obscure STL trivia。

除非职位题目明确要求，否则这些内容的机会成本高于收益。

## 最小训练组合

只做下面六个 mental patterns，基本覆盖第一轮：

| Pattern | Coding/trace | 关键问题 |
|---|---|---|
| filter a `vector` | coding：`stl_safety` | erase 后什么失效？ |
| lower-bound boundary | coding：`binary_search` | invariant 和边界是什么？ |
| bounded heap | coding：`top_k` | 为什么是 min-heap？ |
| frequency/dedup map | paper trace | `operator[]`、rehash、complexity |
| sliding window | paper trace | window invariant 如何维持？ |
| BFS/state queue | paper trace | 何时标记 visited？queue 是否 bounded？ |

第一轮不要新增更多 coding exercises。若 mock test 中某个 paper pattern 连续两次出错，才把它升级为 TODO + tests。

## 90-minute algorithm session

```text
15 min  container choice + complexity recall
20 min  trace hash/sliding-window/BFS 各一个小例子
40 min  完成一个现有 coding TODO
10 min  invalidation/comparator 边界题
 5 min  mistake log
```

Senior-level 回答模板：先陈述 input contract 和 invariant，再给 data structure、time/space complexity、invalidation，
最后说明为什么没有选择更复杂的数据结构。
