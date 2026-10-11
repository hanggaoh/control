# Foundations：建立 ownership、state 与 lifetime 的完整模型

这里从 C++ 的机制出发：对象如何存在、资源由谁管理、操作如何改变状态、失败怎样传播，
最后再连接 container 和 async system。题目用于验证模型，不作为讲解的主线。

| 阅读顺序 | 概念 | 在整体模型中的作用 |
|---|---|---|
| 1 | [Borrowed lifetime](BORROWED_LIFETIME.md) | 分开 object、storage、owner 与 observer |
| 2 | [Exception safety](EXCEPTION_SAFETY.md) | 理解状态、invariant、unwinding 与失败 contract |
| 3 | [Move 与 relocation](MOVE_AND_RELOCATION.md) | 从表达式选择推导 ownership transfer 与容器行为 |
| 4 | [Move assignment 与 aliasing](SELF_MOVE.md) | 理解已有 owner 的替换与 self-move |
| 5 | [Shared ownership](SHARED_OWNERSHIP.md) | 把 lifetime 延伸到多个 owner 与 async callbacks |

这些机制组成一条因果链：object lifetime 决定清理时机；RAII 绑定 ownership；invariant 定义合法状态；
exception safety 规定失败后留下什么；move 交接 ownership；shared ownership 协调跨 scope 的 lifetime。
同步和 cancellation 再为异步系统补上并发与业务状态约束。

先用自己的话画出这个模型，再进入 [专项题集](../FOLLOW_UP_QUIZ.md) 和各页末尾的已有 coding exercises。
代码实现、验收测试和面试练习仍保持独立，不在概念页提供完整 learner solution。
