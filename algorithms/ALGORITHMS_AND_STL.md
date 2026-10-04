# Algorithms, Data Structures and STL

笔试前采用 Pareto 范围，不按完整教科书展开。核心清单和明确排除项见
[`PARETO_CORE.md`](PARETO_CORE.md)。本文件用于解释这些核心选择背后的 C++ 行为。

## 1. 先定义 contract，再选算法

写代码前先问：input 是否 sorted、是否允许 duplicates、结果顺序是否重要、数据规模是多少、是否允许修改
input、错误和空输入如何表达。算法正确性来自明确 contract，不来自记住一段 loop。

复杂度至少说明：

- time complexity：平均、最坏，必要时 amortized；
- space complexity：额外 storage 是否随输入增长；
- hidden cost：allocation、copy/move、hashing、cache miss；
- invalidation：operation 后哪些 iterator、pointer、reference 失效。

## 2. Container choice

| Container | 适合场景 | 关键性质 |
|---|---|---|
| `vector` | 默认 sequence、遍历、随机访问 | contiguous、cache-friendly；扩容会使全部引用失效 |
| `deque` | 两端 push/pop、queue | 非连续；invalidation rules 比 vector 更复杂 |
| `list` | 必须稳定 iterator 且频繁已知位置 splice | traversal/cache locality 差，通常不是默认选择 |
| `map` | ordered lookup/range query | tree，通常 `O(log n)` |
| `unordered_map` | key lookup，不要求顺序 | 平均 `O(1)`，rehash、hash quality 与 worst case 要考虑 |
| `set` | unique ordered keys | 不要为了“查重”盲目使用，先看 ordering 是否需要 |
| `priority_queue` | 反复取得最大/最小优先级 | heap top `O(1)`，push/pop `O(log n)` |

Big-O 相同不代表实际性能相同。连续内存、较少 allocation 和 cache locality 常使 `vector` 胜过 node-based
container。Senior 回答应同时讨论 asymptotic complexity 和 workload characteristics。

## 3. STL algorithms 优于手写循环的地方

`std::find_if`、`std::sort`、`std::lower_bound`、`std::remove_if`、`std::transform` 等表达 intent，并封装经过验证
的实现。C++20 ranges 能减少 iterator pair 错配，但仍不解决 dangling range 或 invalidated iterator。

删除 `vector` 元素时，`remove_if` 只把保留元素移动到前部并返回 new logical end；真正缩小 container 还需要
`erase`。C++20 的 `std::erase_if` 更直接：

```cpp
std::erase_if(values, [](int value) { return value < 0; });
```

如果手写 erase loop，必须使用 `erase` 返回的下一个有效 iterator，不能在 iterator 已失效后继续 `++it`。

## 4. Iterator/reference invalidation

- `vector` reallocation：所有 iterator、pointer、reference 失效。
- `vector::erase`：被删除位置及其后的 iterator/reference 失效。
- `list` erase：通常只有被删除元素失效。
- `unordered_map` rehash：iterator 失效；元素 reference/pointer 通常仍有效。

不要只问 pointer 是否 non-null；要问它指向的 element 是否仍存在、container operation 是否使 handle 失效。

## 5. Binary search 是 boundary search

比“找到 target”更通用的 mental model 是：在 monotonic predicate 上找第一个 true。`lower_bound` 返回第一个
不小于 target 的位置；未找到 exact value 并不等于算法失败。

推荐使用 half-open range `[first, last)`：空区间自然表示为 `first == last`。midpoint 应避免
`(first + last) / 2` 的整数 overflow：

```cpp
auto middle = first + (last - first) / 2;
```

每次迭代必须严格缩小区间，否则容易在相邻边界 infinite loop。

## 6. Top-K 与 heap

全部排序需要 `O(n log n)`。若只要最大的 `k` 个，可维护大小不超过 `k` 的 min-heap：遍历每个值，heap 未满
则加入；值大于 heap top 时替换。时间 `O(n log k)`，额外空间 `O(k)`。最后若 contract 要求 descending output，
仍需整理这 `k` 个元素。

当 `k` 接近 `n` 时，full sort 或 `partial_sort` 可能更简单且更快；选择应基于数据规模、output ordering 和测量。

## 7. 后续路线

下一组将加入 sliding window、LRU cache、graph traversal 和 TCP frame buffer。它们会分别强化：增量状态、
`list + unordered_map` iterator stability、cycle handling，以及 partial input parsing。
