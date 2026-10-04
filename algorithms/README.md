# Algorithms / Data Structures / STL — 学习入口

这个模块是贯穿整个项目的“钢筋”：算法题不是孤立背模板，而是训练 container choice、complexity、
correctness、memory behavior 和 C++ lifetime/invalidation 判断。

采用 80/20 范围：重点掌握 `vector`、hash table、queue/deque、heap，以及 sorting/binary search、
two pointers/sliding window、BFS/DFS。详细取舍见 [`PARETO_CORE.md`](PARETO_CORE.md)。链表技巧、复杂 DP、
Trie、Union-Find 等不进入笔试前第一轮。

固定学习顺序：

1. **Concept**：阅读 [`ALGORITHMS_AND_STL.md`](ALGORITHMS_AND_STL.md)。
2. **Quiz**：完成 [`EXERCISES.md`](EXERCISES.md) 的选择/判断与 Senior 简答。
3. **Coding**：补全 `exercises/` 内的 TODO。
4. **Tests**：打开对应 `exercise_complete` 后运行验收。
5. **Interview explanation**：解释 correctness、complexity、trade-off 与 invalidation。

## 第一组练习

1. `stl_safety`：erase while iterating、erase-remove/`erase_if`、iterator invalidation。
2. `binary_search_boundary`：half-open range、lower-bound semantics、overflow-safe midpoint。
3. `top_k`：bounded min-heap、`O(n log k)`、边界条件与 output ordering。

这三题是必须写完的 coding core。hashing、sliding window 与 BFS/DFS 第一轮以 trace、口头解释和一个小型
纸笔实现覆盖；只有错题暴露明显缺口时才新增代码练习。

```sh
cmake -S . -B build
cmake --build build --target stl_safety_exercise_tests binary_search_exercise_tests top_k_exercise_tests
ctest --test-dir build -R algorithm_ --output-on-failure
```

TODO 初始状态可编译并显示 `SKIPPED`。不要直接把测试期望硬编码进实现。

## 与工程模块的连接

| 算法/STL 能力 | 后续工程场景 |
|---|---|
| `vector` storage 与 invalidation | TCP receive buffer、batch processing |
| `deque` / queue | bounded work queue、producer-consumer |
| heap / priority queue | timer scheduling、retry deadlines、top-K telemetry |
| hash map | connection/session lookup、deduplication、LRU index |
| binary search | sorted configuration、time-range lookup、protocol tables |
| graph traversal | dependency startup/shutdown、cycle detection |

原则：工程代码中先选择清晰、可证明的方案；只有 profiling 证明需要时才牺牲可读性换性能。
