# Foundations 专项新题（无答案）

先完成 [概念复习](concepts/README.md)，然后关闭概念页答题。本文件不附答案。
建议 30–40 分钟；代码片段用于分析，不要求运行有 undefined behavior 的路径。
假设 C++20，片段所需标准头文件已包含，未展示的普通操作满足题目明确给出的条件。

验收：选择/判断题给结论与一句理由；代码分析指出 ownership、异常失败点或 lifetime；
设计题写 invariant、失败后状态和确定性测试方案。完成后请提交你的回答供 Senior review。

## A. 选择与判断（12 题）

1. **单选**：候选对象 copy 成功，commit 中第二个成员交换抛异常，temporary 正常析构。
   什么能够直接推断？
   A. target 一定恢复；B. 必须检查 target invariant 和状态，不能仅凭析构承诺 strong；
   C. basic guarantee 一定成立；D. 所有对象一定泄漏。
2. **判断**：把一个可能抛异常的 swap 标为 noexcept，就能获得可恢复的 strong guarantee。
3. **判断**：copy-and-swap 没有资源泄漏，就足以证明 strong guarantee。
4. **单选**：T 的 copy constructor 可用，move constructor 是 noexcept(false)。
   `auto&& ref = std::move_if_noexcept(source);` 的 ref type 是？
   A. T&&；B. const T&；C. T；D. 由这次运行是否抛异常决定。
5. **判断**：调用 `std::move_if_noexcept(source)`，即使不使用返回值，也会立即复制 source。
6. **单选**：T 禁止 copy，move 可能抛异常。move_if_noexcept 返回什么？
   A. const T&；B. T&&；C. optional<T>；D. 必定无法编译。
7. **判断**：vector 扩容 move 第二个元素失败后，可以普遍通过复制旧元素恢复原值。
8. **判断**：unique owner 的 self-move contract 必须是 no-op，否则任何实现都不合法。
9. **单选**：`x = std::move(x)` 的 assignment body 中，什么一定成立？
   A. this == &other；B. other 是独立 temporary；C. 已经转移了资源；D. x 已销毁。
10. **判断**：两个 shared_ptr 的 get() 相等，且 use_count() 都是 1，可以证明它们共享 control block。
11. **判断**：通过普通 make_shared 创建对象时，constructor 中可以安全调用 shared_from_this()。
12. **判断**：callback 捕获 shared_from_this() 就同时获得 object lifetime、线程同步与 cancellation 保证。

## B. 代码分析（8 题）

13. 对以下三行，分别说明何时调用 constructor、是否保证转移资源、b 的引用 type 如何决定。

    ```cpp
    auto&& a = std::move(source);
    auto&& b = std::move_if_noexcept(source);
    T destination(b);
    ```

    再比较最后一行改为 `T destination(std::move_if_noexcept(source));`。
    注意 named reference expression 的 value category。

14. 类型只定义 `T(const T&) noexcept`，没有 move constructor，也没有显式 deleted move。
    `is_nothrow_move_constructible_v<T>` 能否为 true？它证明“存在 move constructor”吗？

15. 某 move-only 元素的 move 会先修改 source，再根据故障开关抛异常。
    vector::reserve 触发 relocation 并捕获此异常后，你能对旧元素内容承诺什么？
    能否写测试断言“所有旧元素保持原值”？说明原因。

16. 以下是故意有风险的伪代码。非空 self-move 时逐步画出资源状态。
    假设 close 不修改 handle_，invalid 为 -1。

    ```cpp
    Owner& operator=(Owner&& other) noexcept {
        close(handle_);
        handle_ = other.handle_;
        other.handle_ = invalid;
        return *this;
    }
    ```

    若 close 同时将 handle_ 清空，结论是否改变？你会如何明确 contract？

17. 不执行危险代码，分析 ownership graph：

    ```cpp
    auto first = std::make_shared<Device>();
    std::shared_ptr<Device> second(first.get());
    ```

    有几个 ownership groups？两个变量析构时各自相信什么？如何修正调用方式？

18. 假设 State 有 `int left, right` 两个成员：

    ```cpp
    auto owner = std::make_shared<State>();
    std::shared_ptr<int> a(owner, &owner->left);
    std::shared_ptr<int> b(owner, &owner->right);
    ```

    a、b 的 stored pointer 与 ownership identity 分别如何比较？owner.reset() 后成员仍 alive 吗？
    请使用 C++20 的方法表达 ownership 比较。

19. Session 的 constructor 保存 `saved = weak_from_this()`，factory 稍后返回 make_shared 的结果。
    构造结束后 saved 会自动变有效吗？你会把初始化安排在哪里？

20. 比较两个返回函数：

    ```cpp
    std::string_view label_a() { return "sensor"; }
    std::string_view label_b() {
        std::string text = "sensor";
        return text;
    }
    ```

    各自观察谁的 storage？何时失效？为什么“按值返回”不足以判断安全？

## C. Senior 场景题（4 题）

21. 配置对象含 buffer 和 size。更新时 allocation 可能失败，成功后还要向设备发送命令。
    定义内存状态的 invariant；界定 strong guarantee 范围；设计两个确定的 failure-injection cases。
22. unique handle wrapper 同时支持 move assignment、reset 和 destructor。规定 self-move 行为，
    列出空/非空 owner 转移的测试矩阵，说明如何证明每个有效 handle 恰好释放一次。
23. session 自己保存 callback，callback 捕获 strong self，且永远不被调用或释放。
    画 ownership cycle，解释为何外部 owner.reset() 不足以 shutdown；比较 weak capture 与显式清理方案。
24. executor 中排队的 callback 在 shutdown 后才运行，但 strong self 仍保活。
    如何让 callback 安全地跳过业务更新？说明 lifetime、状态检查和同步分别由什么保证；
    用显式推进 fake executor 的方式描述测试，禁止 arbitrary sleep。

## 回答模板

```text
Question:
Conclusion / choice:
Reason (ownership / lifetime / failure path):
Contract or test, when relevant:
```

批改标准：结论正确、前置条件明确、不把 unspecified effects 当固定结果、不用 UB 作为验证方法。
完成后选一道场景题，用 60 秒英文说明你的设计与 trade-off；本题集不提供标准答案。
