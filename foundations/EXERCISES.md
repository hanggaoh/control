# C++ Foundations — Mistake Review Quiz

先独立完成 A、B 两部分。每道简答题都应说明 invariant、异常路径、ownership 或 lifetime，而不只给结论。
题目只针对本轮实际出现的七个薄弱点。答案只保存在 solution/answers branches，不进入 `main`。

## 语法解码热身

1. `[[nodiscard]]` 是 attribute 还是返回 type？忽略它通常会发生什么？
2. `std::move(x)` 自己是否移动资源？真正的 resource transfer 在哪里发生？
3. `operator=` 与 `override` 分别解决什么问题？为什么它们不能互换？
4. 单参数 constructor 为什么经常需要 `explicit`？
5. `= default` 与空 function body 有什么概念差异？`= delete` 又表达什么？
6. `inline` 是否保证最终 machine code 没有 function call？它的语言层主要作用是什么？
7. template 是“配方”，concept 为这份配方增加了什么？
8. `#ifndef` 为什么在 type checking 之前工作？include guard 防止什么？

## A. 选择题与判断题

1. **单选 — copy-and-swap**：copy constructor 已成功，但 `swap` 交换第一个成员后抛异常。最准确的结论是：
   A. strong guarantee 仍自动成立；B. target 可能部分改变；C. compiler 会 rollback；D. temporary 不会析构。
2. **判断 — vector relocation**：只要类型定义了 move constructor，`vector` 扩容就必须 move，不能 copy。
3. **单选 — reference lifetime**：哪个返回接口是安全的？
   A. `const string& f(){ return string("x"); }`；B. `string f(){ return "x"; }`；
   C. `string_view f(){ string s="x"; return s; }`；D. `const string* f(){ string s="x"; return &s; }`。
4. **判断 — self-move**：unique-resource move assignment 只要先 release `*this` 再接管 `other`，就天然支持
   `x = std::move(x)`。
5. **单选 — control block**：`p1` 和 `p2` 分别由同一个 raw pointer 独立构造。下列何者正确？
   A. 自动合并 control block；B. 相同 `get()` 保证相同 ownership；C. 可能 double delete；D. weak count 会修复它。
6. **单选 — `move_if_noexcept`**：当 copy 可用且 move 可能抛异常时，它通常产生：
   A. `T&&` 并在失败后 retry；B. `const T&` 以选择 copy；C. deep copy 的 temporary；D. runtime transaction log。
7. **判断 — `enable_shared_from_this`**：在已由 `shared_ptr` 管理的对象中，`shared_ptr<T>(this)` 与
   `shared_from_this()` 具有相同 ownership semantics。

## B. Senior interview 简答题

1. 用 prepare → commit → cleanup 三阶段解释 copy-and-swap。为什么 throwing `swap` 会破坏 strong guarantee？
2. Why can `std::vector` prefer copy during reallocation? `noexcept` move 如何影响这个选择和 exception guarantee？
3. 对比 local `const std::string& r = std::string{"sensor"};` 与函数返回该 temporary 的 `const&`。
   lifetime 分别到哪里结束？你会怎样重构返回接口？
4. 展开 `x = std::move(x)` 时 `this` 与 `other` 的 aliasing 关系。release-before-transfer 为什么可能丢失资源？
   给出两种安全实现策略及其 trade-off。
5. 为什么“相同 raw address”不等于“相同 shared ownership”？control block 包含什么？如何比较 ownership identity，
   `weak_ptr` 又如何帮助打破 cycle？
6. 解释 `std::move_if_noexcept` 的决定发生在 compile time/type selection 还是 runtime。为什么无法可靠地
   “先 move，失败后 rollback 再 copy”？
7. Why is `shared_ptr<T>(this)` dangerous? 说明 `enable_shared_from_this` 的前置条件，并比较 async callback
   捕获 strong `self` 与捕获 `weak_from_this()` 的 lifetime/cancellation semantics。

## C. Coding 顺序

1. [`exercises/copy_swap/README.md`](exercises/copy_swap/README.md)
2. [`exercises/move_lifetime/README.md`](exercises/move_lifetime/README.md)
3. [`exercises/unique_resource/README.md`](exercises/unique_resource/README.md)
4. [`exercises/shared_ownership/README.md`](exercises/shared_ownership/README.md)

推荐循环：Concept → Quiz（不看答案）→ Coding → Tests → 用 60 秒英文解释设计与 failure path。
