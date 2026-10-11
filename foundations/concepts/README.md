# Foundations 错题专项学习路线

每次只学一页：读概念 → 用自己的话复述 → 完成已有 coding exercise → 跑验收测试 → 回答追问 → 用英文总结。
先复述再写代码；卡住时记录自己对 failure path 的假设。

| 顺序 | 概念文件 | 对应错题 | 已有 coding exercise |
|---|---|---|---|
| 1 | [异常安全与 copy-and-swap](EXCEPTION_SAFETY.md) | A1 | [copy_swap](../exercises/copy_swap/README.md) |
| 2 | [move、noexcept 与 relocation](MOVE_AND_RELOCATION.md) | A2、A6 | [move_lifetime](../exercises/move_lifetime/README.md) |
| 3 | [self-move 与资源转移](SELF_MOVE.md) | A4 | [unique_resource](../exercises/unique_resource/README.md) |
| 4 | [control block 与 async lifetime](SHARED_OWNERSHIP.md) | A5、A7 | [shared_ownership](../exercises/shared_ownership/README.md) |
| 5 | [借用与 lifetime](BORROWED_LIFETIME.md) | A3，巩固已答对的知识 | [move_lifetime](../exercises/move_lifetime/README.md) |

读完后做 [专项新题](../FOLLOW_UP_QUIZ.md)。不先查看旧题答案；每题除了结论，再写一句理由。
这组资料不新增 coding TODO，也不提供已有练习的完整实现；已有 CMake/验收测试继续使用。

背景迁移：Volkswagen 的 C++/NDK/JNI 和实时视频 pipeline 可用于讨论 buffer ownership、callback lifetime；
Parkable 的 Linux/IoT 和生产可靠性经验可用于讨论失败恢复、资源清理与可观测性。
工业通信 session 是学习场景，不代表已有 SCADA/DNP3 生产经验。
