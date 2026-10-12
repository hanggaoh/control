# C++ Foundations — Mistake Review Quiz

先独立完成 A、B 两部分。每道简答题都应说明 invariant、异常路径、ownership 或 lifetime，而不只给结论。
题目只针对本轮实际出现的七个薄弱点。答案只保存在 solution/answers branches，不进入 `main`。

## 语法解码热身

1. `[[nodiscard]]` 是 attribute 还是返回 type？忽略它通常会发生什么？

    **答案：** It is an attribute. Ignoring the return value usually triggers a compiler warning.

    **说明：** 具体诊断取决于编译器及配置，因此通常产生 warning，不保证编译失败。

2. `std::move(x)` 自己是否移动资源？真正的 resource transfer 在哪里发生？

    **答案：** std::move does not move resources itself. Resource transfer happens in the move constructor or move assignment operator, if one is selected and implemented to transfer resources.

    **说明：** std::move 是类型转换，允许选择 move overload；不保证实际发生移动，也可能调用 copy。

3. `operator=` 与 `override` 分别解决什么问题？为什么它们不能互换？

    **答案：** operator= defines assignment behavior. override checks that a member function overrides a base-class virtual function. They serve different purposes.

    **说明：** operator= 是赋值运算符函数的名字；override 是覆盖检查标记。二者用途不同，不能互相替代。

4. 单参数 constructor 为什么经常需要 `explicit`？

    **答案：** explicit prevents unintended implicit conversions through the constructor.

    **说明：** implicit conversion 指隐式转换。对于 explicit Seconds(int)，Seconds a{5}; 可以，Seconds b = 5; 不可以。

5. `= default` 与空 function body 有什么概念差异？`= delete` 又表达什么？

    **答案：** = default asks the compiler to generate the default implementation. An empty function body is a user-provided implementation. = delete forbids calling the function.

    **说明：** 默认 copy constructor 会复制成员；手写空的 copy constructor 不会自动复制成员。空函数体也不意味着成员不初始化：成员仍遵循初始化规则。

6. `inline` 是否保证最终 machine code 没有 function call？它的语言层主要作用是什么？

    **答案：** inline does not guarantee inline expansion. Its main language-level role is to allow definitions in multiple translation units, subject to ODR requirements.

    **说明：** 展开是用函数体替代函数调用。inline 的语言语义支持在 header 中定义函数并被多个 .cpp 包含，但必须满足 One Definition Rule；它不是任意重复定义的许可。

7. template 是“配方”，concept 为这份配方增加了什么？

    **答案：** A concept adds compile-time constraints to template parameters, expressing requirements on the accepted types.

    **说明：** template 是配方，concept 规定材料要求。例如 template <std::integral T> T twice(T value) { return value * 2; } 使用 <concepts> 中的 std::integral：twice(3) 合法，twice(3.5) 不满足约束。concept 也参与 overload selection，但不自动证明业务逻辑正确。

8. `#ifndef` 为什么在 type checking 之前工作？include guard 防止什么？

    **答案：** #ifndef is handled during preprocessing, before C++ type checking. An include guard prevents repeated inclusion of a header within one translation unit.

    **说明：** 先 preprocessing，再 compilation/type checking。第一次包含 header 时定义 guard macro；后续包含时 #ifndef 条件不成立，跳过内容。它不阻止不同 .cpp 分别包含同一 header，也不解决所有链接阶段重复定义问题。

## A. 选择题与判断题

1. **单选 — copy-and-swap**：copy constructor 已成功，但 `swap` 交换第一个成员后抛异常。最准确的结论是：
   A. strong guarantee 仍自动成立；B. target 可能部分改变；C. compiler 会 rollback；D. temporary 不会析构。

   **答案：B。** strong exception guarantee 要求失败后目标状态不变。若 `swap` 部分完成后抛异常，target 可能已改变，编译器不会自动回滚。copy-and-swap 通常依赖不抛异常的 `swap` 完成 commit；栈展开时 temporary 仍会析构，但析构不等于撤销交换。

2. **判断 — vector relocation**：只要类型定义了 move constructor，`vector` 扩容就必须 move，不能 copy。

   **答案：False。** 当 copy 可用而 move 可能抛异常时，`vector` 扩容可以选择 copy 来维持异常保证。这是操作前根据类型性质进行的选择，不是先尝试 move、捕获异常后再 copy。如果 copy 不可用，则可能必须使用可能抛异常的 move，此时异常保证可能减弱。

3. **单选 — reference lifetime**：哪个返回接口是安全的？
   A. `const string& f(){ return string("x"); }`；B. `string f(){ return "x"; }`；
   C. `string_view f(){ string s="x"; return s; }`；D. `const string* f(){ string s="x"; return &s; }`。

   **答案：B。** 按值返回的 `string` 拥有自己的内容。A 返回的引用指向已结束生命周期的 temporary；C 的 `string_view` 不拥有字符数据，local string 销毁后 view 悬空；D 返回 local string 的地址，函数返回后 pointer 悬空。关键是 object lifetime 已结束，而不是内存是否还保留原来的字节。

4. **判断 — self-move**：unique-resource move assignment 只要先 release `*this` 再接管 `other`，就天然支持
   `x = std::move(x)`。

   **答案：False。** self-move 时 `this` 和 `other` 指向同一个对象，不能把它们当成两个独立资源所有者。仅凭 release-before-transfer 顺序无法保证安全：结果取决于释放后是否清空 handle、后续是否读取失效 handle 等实现细节。可以先检查 `this != &other`，让 self-move 成为 no-op；也可设计经过验证的 temporary-and-swap 流程。转移后不能再关闭已交给目标的同一资源，必须避免 double release、泄漏和悬空 handle。

5. **单选 — control block**：`p1` 和 `p2` 分别由同一个 raw pointer 独立构造。下列何者正确？
   A. 自动合并 control block；B. 相同 `get()` 保证相同 ownership；C. 可能 double delete；D. weak count 会修复它。

   **答案：C。** 两次从 raw pointer 独立构造 `shared_ptr` 会建立独立 control blocks；使用通常的删除器时，两个 ownership groups 都会尝试删除同一对象。应通过复制已有 `shared_ptr` 来共享 ownership，而不是用它的 `get()` 再构造一个 owner。

6. **单选 — `move_if_noexcept`**：当 copy 可用且 move 可能抛异常时，它通常产生：
   A. `T&&` 并在失败后 retry；B. `const T&` 以选择 copy；C. deep copy 的 temporary；D. runtime transaction log。

   **答案：B。** `std::move_if_noexcept(x)` 在此条件下返回 `const T&`，使后续构造选择 copy constructor；它自己不创建副本。若 move 为不抛异常，或 copy 不可用，则返回 `T&&`。选择依据是编译期 type traits，不是运行时异常重试。

7. **判断 — `enable_shared_from_this`**：在已由 `shared_ptr` 管理的对象中，`shared_ptr<T>(this)` 与
   `shared_from_this()` 具有相同 ownership semantics。

   **答案：False。** `shared_ptr<T>(this)` 创建新的 control block，与现有 owner 独立，可能导致 double delete。正确使用 `enable_shared_from_this<T>` 时，`shared_from_this()` 返回共享现有 control block 的 `shared_ptr`。前提是 public、无歧义地继承相应基类，且对象已由合适的 `shared_ptr` 建立 shared ownership；不能在构造函数中依赖这一点。

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
