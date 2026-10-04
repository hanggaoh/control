# C++ Foundations — 学习入口

本模块固定为三层，按顺序学习：

1. **概念层**：本页建立地图；[`MODERN_CPP_SUMMARY.md`](MODERN_CPP_SUMMARY.md) 系统讲解。
2. **问题层**：[`EXERCISES.md`](EXERCISES.md) 用 conceptual questions 检查 mental model。
3. **实践层**：[`exercises/`](exercises/) 中是可编译的 TODO exercises 和验收测试。

不要先看完整实现。先口头回答问题，再补代码，最后运行测试。

这个模块不是语法百科。目标是建立一套能解释代码、定位错误和讨论设计的 mental model，
为后面的 `RAII`、Boost.Asio、concurrency、Windows/MFC 和 legacy modernization 打基础。

## 1. 从 source code 到 running program

```text
source/header
    ↓ preprocessing     (#include, #define, conditional compilation)
translation unit
    ↓ compilation       (type checking, template instantiation, code generation)
object file
    ↓ linking           (resolve definitions and symbols)
executable
    ↓ runtime           (object lifetime, threads, I/O, cleanup)
```

理解这条链能区分三类常见问题：

- compile error：类型不匹配、不可见的 declaration、语法错误
- link error：有 declaration，但缺少 definition；或违反 ODR
- runtime error：dangling reference、data race、resource leak、invalid state

## 2. Name、type、object 与 lifetime

- `declaration`：向编译器介绍 name 和 type，不一定产生实体。
- `definition`：真正定义 function、class、variable；variable definition 通常分配 storage。
- `initialization`：对象 lifetime 开始时建立初始状态。
- `assignment`：对象已经存在后，用新值替换旧状态。
- `scope`：name 在哪里可见。
- `lifetime`：object 在什么时候真实存在。

Senior-level C++ 讨论经常不是“这个 pointer 是否为 null”，而是：谁拥有 object、它何时销毁、
异步 callback 执行时它是否仍然 alive。

## 3. Value semantics 与 ownership

优先级通常是：

1. value member：最简单，lifetime 跟随 enclosing object。
2. `std::unique_ptr<T>`：exclusive ownership，需要 dynamic lifetime 或 polymorphism。
3. `std::shared_ptr<T>`：shared ownership 确实存在时使用，不是通用 lifetime 修补工具。
4. raw pointer/reference：通常表达 non-owning observation，必须有清晰 lifetime contract。

`RAII` 把 resource lifetime 绑定到 object lifetime：constructor 建立 invariant，destructor 释放资源。
resource 不只包括 memory，也包括 file、socket、mutex lock、thread、timer 和 registration token。

## 4. Functions、classes 与 interfaces

- 使用 `const` 表达只读 contract。
- 使用 `noexcept` 表达不会抛出异常的 operation，尤其是 destructor 和 move operation。
- 使用 `explicit` 防止意外 implicit conversion。
- 小接口、明确 invariant，优于暴露内部数据。
- runtime polymorphism 使用 virtual interface；compile-time polymorphism 使用 template/concept。
- composition 通常比深 inheritance hierarchy 更容易测试和演进。

## 5. Generic programming

- template 是 compiler 生成 concrete specialization 的 recipe。
- template definition 通常必须对 instantiation point 可见，因此多放在 header。
- C++20 `concept` 为 template parameter 提供可读 constraint 和更好的 diagnostics。
- `constexpr` 允许 computation 在 compile time 发生，但不保证每次都在 compile time 执行。

## 6. Error handling

- exception：适合 constructor failure 或无法在当前层处理的 failure。
- `std::optional<T>`：有值或无值，但不携带丰富错误原因。
- error code / expected-like result：错误是正常 control flow，调用方需要具体原因。
- assertion：程序员违反 invariant，不应替代生产错误处理。

关键不是统一使用一种机制，而是建立一致、可观察、不会丢失上下文的 error policy。

## 7. Concurrency

需要分别思考：

- shared state 由哪个 mutex 保护？
- wait predicate 是什么？是否处理 spurious wakeup？
- shutdown 如何唤醒 blocked operations？
- thread/handler lifetime 是否超过其引用对象？
- callback 是否可能并发执行？顺序由 mutex、strand 还是 event loop 保证？

`atomic` 只保护一次 atomic operation；它不会自动保护跨多个变量的 invariant。

## 8. 如何连接到后续技术

| 基础概念 | RAII | Boost.Asio | MFC / legacy modernization |
|---|---|---|---|
| lifetime | destructor 释放 handle | handler 捕获对象必须 alive | UI object 与 service lifetime 分离 |
| ownership | move-only wrapper | `shared_from_this` 管理 async session | raw owning pointer 改为 value/`unique_ptr` |
| interface | resource policy | transport/service abstraction | thin MFC adapter + pure C++ core |
| concurrency | scoped lock | strand、timer、cancellation | UI thread affinity |
| errors | constructor invariant | `error_code` / completion handler | boundary translation + logging |
| testing | fake closer | fake transport / deterministic executor | characterization tests |

## Runnable examples and exercises

[`include/cpp_foundations.hpp`](include/cpp_foundations.hpp) 展示 `inline constexpr`、template、
value member、`span`、`optional` 和 `unique_ptr`。测试展示 declaration/definition 分离和
compile-time checks。

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build -R cpp_foundations --output-on-failure
ctest --test-dir build -R foundation_ --output-on-failure
```

TODO 练习初始状态也可构建。测试会报告 `SKIPPED`；完成一个练习后，把对应 header 中的
`exercise_complete` 改为 `true`，重新构建并运行，测试就会执行验收。

当前 mistake-review coding 顺序是 `copy_swap` → `move_lifetime` → `unique_resource` →
`shared_ownership`。它们分别覆盖 transactional assignment、relocation/reference lifetime、self-move，
以及 control-block/async callback lifetime。现有 `exercises/01_unique_handle` learner code 不属于本模块，
这些练习不会修改或替代它。

## Short check

请用自己的话解释：`declaration`、`definition` 和 `initialization` 分别发生了什么？为什么
“声明了一个 function”仍可能得到 link error？

详细的新旧 C++ 对照见 [`MODERN_CPP_SUMMARY.md`](MODERN_CPP_SUMMARY.md)。
