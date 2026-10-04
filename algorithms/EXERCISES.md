# Algorithms / STL — Conceptual Exercises

先独立答题。简答题需要说明 correctness、complexity 和 invalidation。答案只保存在
solution/answers branches，不进入 `main`。

## A. 选择题与判断题

1. **判断**：`vector` 扩容后，原先保存的 element pointer 只要非空就仍可使用。
2. **单选**：`remove_if` 对 `vector` 的作用是：A. 立即缩小 size；B. 移动保留元素并返回 logical end；
   C. 释放 capacity；D. 保证不移动元素。
3. **单选**：在 sorted range 中查找第一个 `>= target` 的位置，应优先使用：A. `find`；B. `lower_bound`；
   C. `upper_bound`；D. `partition`。
4. **判断**：binary search 的 midpoint 永远可以安全写成 `(left + right) / 2`。
5. **单选**：从 `n` 个值中选最大 `k` 个且 `k << n`，典型方案是：A. size-k min-heap；
   B. size-k max-heap；C. linked list linear scan；D. hash set。
6. **判断**：`unordered_map` lookup 在所有输入下都保证 worst-case `O(1)`。

## B. Senior interview 简答题

1. 为什么 `vector` 通常是 sequence 的默认选择？什么具体 requirement 会让你改选 `deque` 或 `list`？
2. 解释 erase-remove idiom。手写 `for` loop 一边遍历 `vector` 一边 erase 时，最常见的 invalidation bug 是什么？
3. 用 loop invariant 解释 half-open binary search：每轮开始时答案为什么仍在 `[first, last)`？
4. 比较 full sort、`partial_sort` 和 size-k heap 实现 top-K 的复杂度与适用场景。
5. `unordered_map` 的 average complexity、rehash invalidation 和 adversarial input 分别意味着什么？
6. 在 TCP receive buffer 中使用 `vector<std::byte>` 时，哪些 append/erase 操作可能使 parser 保存的 view 失效？

