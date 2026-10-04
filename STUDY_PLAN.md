# Written Test Sprint Plan

目标：在笔试前完成一轮 breadth-first 复习。每个模块只达到“能判断、能写基础实现、能解释 trade-off”，不在第一轮
追求完整掌握。每天可投入完整 8 小时，但按 4 个高质量 session 组织，避免连续 8 小时低效输入。

## 1. 固定训练节奏

### Eight-hour study day

| 时间 | 内容 |
|---|---|
| 09:00–10:30 | Session 1：核心概念 + closed-book recall |
| 10:30–10:45 | 离屏休息 |
| 10:45–12:15 | Session 2：核心 coding exercise |
| 12:15–13:15 | 午饭和走动，不看课程 |
| 13:15–14:45 | Session 3：第二练习或 failure-path tracing |
| 14:45–15:00 | 离屏休息 |
| 15:00–16:30 | Session 4：tests、quiz、interview explanation |
| 16:30–17:00 | mistake log、commit、安排下一天 |

这是 8 小时日程，其中约 6 小时为 focused work。不要把午休压缩成继续看文档；后半天的 debugging 质量比表面学习
时长更重要。

每个 90 分钟 session 内部：

| 时间 | 内容 | 输出 |
|---:|---|---|
| 10 min | closed-book recall | 写出 3–5 个关键点，不查文档 |
| 20 min | concept scan | 只修正 recall 中的缺口 |
| 40 min | one coding TODO | 编译并运行 focused tests |
| 15 min | quiz + explanation | 选择/判断 + 60 秒口头回答 |
| 5 min | mistake log | 记录一个错误、原因和正确 rule |

每天最多四个 session。Session 4 不开启新主题，只做 tests、错题和解释。每 45–50 分钟短暂离开屏幕。

## 2. Module exit criteria

满足下面四项就离开模块，禁止因为“还可以再深入”而延期：

1. quiz 首轮达到约 80%，并能解释错题；
2. 至少一个核心 TODO 通过 tests；
3. 能在 60–90 秒内说明 ownership/lifetime/complexity/failure path；
4. mistake log 留下一条可在 D-2 重测的问题。

时间到但代码未完成：保存 failing test、写明 blocker，然后继续下一模块。D-3 统一回补 P0 blocker。

## 3. Depth budget

### P0 — 笔试前必须完成

- Foundations：RAII、copy/move、exception safety、lifetime、smart pointers；
- STL/Algorithms：只按 `algorithms/PARETO_CORE.md`，重点是 vector、hash、queue、heap、binary search、
  sliding window、BFS/DFS；
- Concurrency：mutex/CV、predicate、shutdown、data race；
- Networking：TCP stream/framing、partial read/write、timeout、reconnect；
- Boost.Asio：handler lifetime、strand、timer/cancellation、graceful shutdown；
- MFC/legacy：UI thread affinity、thin adapter、characterization test、incremental refactor。

### P1 — 必须看懂，完成一个小练习

- Control systems：sample time、PID saturation/anti-windup、filter、hysteresis、interlock；
- Database：SQL filter/join/group、index、transaction/isolation、RAII transaction boundary；
- Distributed systems：partial failure、timeout/retry/idempotency/ordering，只做 scenario reasoning；
- Idioms/patterns：RAII、Rule of Zero/Five、scope guard、PImpl，以及 Strategy/State/Adapter/Observer；
- Capstone：dependency direction、thread ownership、shutdown sequence；
- templates/concepts、basic Windows resource ownership。

### P2 — 笔试后再深入

- advanced dynamic programming、复杂 graph algorithms；
- lock-free structures 与 memory-order 推导；
- advanced control tuning、state-space/observer、真实 grid protection；
- 完整 IEC 61850/DNP3 implementation；
- MFC visual polish、复杂 custom controls；
- Boost.Asio 高级 allocator/executor customization。

## 4. Seven-day first-pass sprint

8 小时投入下，第一轮覆盖压缩为 7 天。无论距离笔试还有多少天，先完成这一轮，再进入第二轮，而不是把单个模块
摊开到多天。

| Day | Session 1 | Session 2 | Session 3 | Session 4 / Exit deliverable |
|---|---|---|---|---|
| Day 1 | Baseline quizzes | copy-swap/move | lifetime/shared ownership | Foundations tests + 90 秒解释 |
| Day 2 | STL/container choice | binary search | top-K + `unique_handle` | complexity/invalidation quiz |
| Day 3 | mutex/CV mental model | bounded queue | shutdown/race scenarios | concurrency tests + tracing |
| Day 4 | TCP stream/framing | parser coding | timeout/retry/backoff | distributed partial-failure scenarios |
| Day 5 | Asio async lifecycle | read/write/timers | cancellation/reconnect | draw lifetime + shutdown sequence |
| Day 6 | MFC/UI isolation | C++ idioms/patterns | control essentials | UI-thread + interlock scenarios |
| Day 7 | Database 80/20 | Capstone system-design TODO (90 min) | 90-minute mock | mock review + first-pass checkpoint |

每一天只允许一个主模块和一个小型关联模块。当天未完成的 optional exercise 不顺延；只有 P0 blocker 可以进入 Day 7。

## 5. Remaining-days strategy

### 距离笔试 8–14 天

完成 7 天 first pass 后：

| Remaining day | Focus |
|---|---|
| Pass 2 / Day 1 | Foundations + Concurrency 错题重测，不重读全文 |
| Pass 2 / Day 2 | TCP + Asio failure-path coding/tracing |
| Pass 2 / Day 3 | Algorithms + Control 边界题 |
| Pass 2 / Day 4 | MFC/legacy + Capstone architecture explanation |
| D-3 | 只修 P0 blockers，合并已 review branches 到 `answers` |
| D-2 | 第二次 90-minute mock + targeted correction |
| D-1 | 最多 3 小时轻复习，其余休息 |

多出来的时间用于 retrieval practice 和 mock，不升级到 P2 深度。

### 距离笔试正好 7 天

直接执行 Seven-day first-pass sprint。Day 7 的 mock 不得取消。

### 只有 4–6 天

- Day 1：Foundations + RAII + STL；
- Day 2：Concurrency + bounded queue；
- Day 3：TCP + Boost.Asio；
- Day 4：MFC + Control + Capstone；
- 倒数第二天：P0 blockers + timed mock；
- D-1：mistake log 和轻复习。

若只有 4 天，MFC/Control 以 scenario 和口头解释为主，不强求完成所有 coding TODO。

笔试当天只做 15–20 分钟 recall：ownership、CV predicate、TCP framing、Asio lifetime、UI thread boundary。不要临时学习
新 API。

### 少于 4 天

停止新增实现。优先 closed-book tracing、已有 tests、mistake review 和以下五个 scenario：

1. copy/move/exception failure path；
2. condition-variable shutdown；
3. partial TCP frame；
4. Asio callback lifetime/cancellation；
5. MFC UI close 时的 cross-thread shutdown。

## 6. Branch rhythm

- `main`：只更新题目、计划、TODO 和 tests；
- 每个模块从 `main` 建立 `solution/<module>`；
- 一个 session 最多产生一个 focused commit，一天最多 3 个 code commits；
- 达到 exit criteria 后停止该 branch 的第一轮，切换下一模块；
- D-3 才把已 review 的 branches 合并进 `answers`；
- 不为了制造全绿而在 `main` 填答案。

时间紧时不要在同一个 checkout 频繁 stash。可用 worktree 为当前模块保留独立目录，但同时 active 的 solution 模块最多两个。

## 7. Daily tracker

每天结束只记录这一行，不写长篇学习日志：

```text
Day: __ | Sessions: __/4 | Module: ____ | Quiz: __% | Test: pass/fail | Mistake: ____ | Next: ____
```

### Completion checklist

- [ ] Foundations
- [ ] Algorithms/STL
- [ ] RAII/resource ownership
- [ ] Concurrency/bounded queue
- [ ] TCP framing/network reliability
- [ ] Boost.Asio lifecycle
- [ ] MFC/legacy isolation
- [ ] Control systems essentials
- [ ] Capstone architecture walkthrough
- [ ] Timed mock test
