# C++ Idioms and Practical Patterns — 70/30

这个模块不是 GoF 背诵课。目标是用半天建立一套能解释 ownership、lifetime、exception safety 和 change boundary
的设计词汇：约 70% C++ idioms，30% practical design patterns。

1. 阅读 [`IDIOMS_AND_PATTERNS.md`](IDIOMS_AND_PATTERNS.md)。
2. 完成 [`EXERCISES.md`](EXERCISES.md) 的判断与设计题。
3. 按 [`PRACTICE_MAP.md`](PRACTICE_MAP.md) 回到已有代码练习，不新增重复 demo。
4. 在 Capstone architecture 中指出 pattern 的位置与代价。

答案放在 `solution/idioms-patterns`，review 后合并到 `answers`。

## 半天时间盒

| 时间 | 内容 |
|---:|---|
| 45 min | RAII、Rule of Zero/Five、copy-and-swap、scope guard |
| 30 min | PImpl、type erasure、non-virtual interface、dependency injection |
| 45 min | Strategy、State、Adapter、Observer/Command |
| 45 min | existing-code review + Capstone mapping |
| 15 min | quiz、错题和 60 秒英文回答 |

退出条件：能从具体 failure/change requirement 推导 idiom/pattern，而不是看到名称就套 class hierarchy。
