# Catapult Senior C++ Developer Interview Prep

这是 Hang Gao 的 hands-on C++ interview preparation repository，目标岗位侧重
industrial automation、Windows C++、Boost.Asio、networking、concurrency、legacy
modernization 和 high reliability。

教学采用：concept → short check → runnable exercise → senior-level review →
interview follow-up → concise English answer。代码与 technical vocabulary 使用英文，
解释使用中文。

笔试冲刺的时间盒、模块优先级、每日节奏和完成标准见 [`STUDY_PLAN.md`](STUDY_PLAN.md)。计划按每天 8 小时、
7 天完成第一轮覆盖；知道实际笔试日期后再映射到具体日期并安排第二轮。

题目与答案使用分支隔离：`main` 只保存概念、题目、TODO 和 tests；每个模块在独立 solution branch
完成，最后合并进 `answers` 集成分支。具体命名和操作见 [`BRANCH_WORKFLOW.md`](BRANCH_WORKFLOW.md)。

## Foundations

先阅读 [`foundations/README.md`](foundations/README.md) 建立 compile/link、object lifetime、
ownership、template 和 concurrency 的高层概念地图。传统 C++ 与 Modern C++ 的重点差异见
[`foundations/MODERN_CPP_SUMMARY.md`](foundations/MODERN_CPP_SUMMARY.md)。
概念学完后，用 [`foundations/EXERCISES.md`](foundations/EXERCISES.md) 做口头题，再进入
[`foundations/exercises`](foundations/exercises) 的可编译 TODO 练习。

## Algorithms / Data Structures / STL

[`algorithms/README.md`](algorithms/README.md) 是贯穿项目的算法骨架，不做孤立刷题：每个练习同时要求
说明 complexity、container choice、iterator/reference invalidation 和工程场景。第一组覆盖 STL 安全修改、
binary-search boundary 与 heap-based top-K，后续会连接 bounded queue、TCP framing 和 timer scheduling。

## Control Systems — Power/Industrial Track

[`control_systems/README.md`](control_systems/README.md) 是岗位定向复习分支，将本科控制论与工业/电力 C++
工程连接起来。第一阶段覆盖 sampled-data mental model、PID、saturation/anti-windup、filtering、hysteresis、
interlock 与 deterministic execution；不把 SCADA supervisory logic、fast protection 和 closed-loop control 混为一谈。

## Database — 80/20 Track

[`database/README.md`](database/README.md) 聚焦工业 C++ 最常用的数据库能力：SQL 查询与聚合、schema/index、
transaction/isolation、parameter binding，以及 telemetry/alarm 数据的 timestamp、quality、retention 和 store-and-forward。
它只占半天到一天，不扩展到数据库内核或 DBA 深度。

## Concurrency and Distributed Systems — 80/20

- [`concurrency/README.md`](concurrency/README.md)：thread ownership、mutex/CV、data race、shutdown；唯一必写核心是
  已有 bounded queue，不新增 lock-free 深坑。
- [`distributed_systems/README.md`](distributed_systems/README.md)：partial failure、timeout、retry、idempotency、
  ordering、consistency 与 observability；第一轮只做 scenario reasoning，不另建大型服务。

## C++ Idioms and Practical Patterns

[`idioms_patterns/README.md`](idioms_patterns/README.md) 用半天复习高频 C++ idioms 与少量工程设计模式。比例约为
70% idioms、30% patterns；练习复用现有 Foundations、Database、Control、MFC 和 Capstone 内容，不新增模式 demo 项目。

## Final Capstone — Boost.Asio + MFC

[`capstone/README.md`](capstone/README.md) 定义最终合成项目：一个可模拟的工业控制站。pure C++ core 负责
protocol、state、control 与 safety rules；Boost.Asio adapter 负责 async TCP lifecycle；MFC adapter 只负责
Windows UI 和 message dispatch。它会把前面所有模块串成可测试、可演示、可用于 Senior interview 讲解的系统。

## Networking, Boost.Asio, and MFC practice

- [`networking/README.md`](networking/README.md)：TCP stream、framing、partial/multiple frames。
- [`boost_asio/README.md`](boost_asio/README.md)：async lifecycle、generation token、reconnect/cancellation。
- [`mfc_legacy/README.md`](mfc_legacy/README.md)：UI thread boundary、dispatcher seam、legacy isolation。
- [`mock_exams/README.md`](mock_exams/README.md)：两套 90 分钟模拟笔试。
- [`MISTAKE_LOG.md`](MISTAKE_LOG.md)：统一错题与复测记录。

## Exercises

1. [`01_unique_handle`](exercises/01_unique_handle/README.md) — C++20 RAII socket/file-handle wrapper
2. [`02_bounded_queue`](exercises/02_bounded_queue/README.md) — bounded multi-producer/multi-consumer queue

后续路线：

3. raw-pointer legacy refactor
4. TCP framing and partial reads
5. Boost.Asio async TCP lifecycle
6. Qt/MFC business-logic isolation
7. characterization tests and incremental modernization
8. SCADA/DNP3 reliability scenarios

## Build and test

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

第一个练习有意从 failing tests 开始。完成 TODO 后，目标是让测试全部通过。

只运行某个练习：

```sh
ctest --test-dir build -R exercise_01_unique_handle --output-on-failure
ctest --test-dir build -R bounded_queue --output-on-failure
ctest --test-dir build -R cpp_foundations --output-on-failure
ctest --test-dir build -R foundation_ --output-on-failure
ctest --test-dir build -R algorithm_ --output-on-failure
ctest --test-dir build -R control_ --output-on-failure
ctest --test-dir build -R networking_ --output-on-failure
ctest --test-dir build -R asio_ --output-on-failure
ctest --test-dir build -R mfc_ --output-on-failure
```

Foundations TODO tests 初始会显示 `SKIPPED` 并通过构建；完成对应 header 的 TODO 后，将
`exercise_complete` 切换为 `true` 以启用完整验收。

## Start a learning session

在新的 Codex chat 中打开此目录，然后输入：

> Start exercise 1. Give me the requirements only. Review my solution as a Senior C++ interviewer.

项目约定和经历边界记录在 [`AGENTS.md`](AGENTS.md)。
