# Basic C++ Syntax

顺序：[`CONCEPTS.md`](CONCEPTS.md) → [`QUIZ.md`](QUIZ.md) → `include/basic_syntax.hpp` TODO → tests。
每组 20–30 分钟，先闭卷判断，再补代码。七组分别是 initialization/type deduction、pointer/reference、functions、
classes、value categories、polymorphism、templates/build boundaries。

`main` 保留题目与 TODO。答案写在 `solution/foundations`，合并 `main` 的新题目后再练习，review 后汇入 `answers`。

```sh
cmake -S . -B build
cmake --build build --target basic_syntax_exercise_tests
ctest --test-dir build -R foundation_basic_syntax --output-on-failure --verbose
```

练习按组启用：完成一组后只把该组 `*_complete` 改为 `true`。输出会分别列出 PASS/FAIL/SKIPPED，全部跳过不代表掌握。

代码练习覆盖七组基础行为；header/link/ODR 使用 quiz 讨论，不通过执行 UB 验证。
