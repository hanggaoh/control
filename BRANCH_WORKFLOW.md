# Exercise and answer branch workflow

## Branch roles

| Branch | Purpose | May contain completed answers? |
|---|---|---|
| `main` | concepts, quiz questions, TODO code, tests, build configuration | No |
| `solution/foundations` | completed Foundations quiz and coding exercises | Yes |
| `solution/algorithms` | completed Algorithms/STL quiz and coding exercises | Yes |
| `solution/control-systems` | completed industrial control theory and coding exercises | Yes |
| `solution/database` | completed SQL and C++ database-boundary exercises | Yes |
| `solution/concurrency` | completed concurrency quiz and bounded-queue review | Yes |
| `solution/distributed-systems` | completed distributed-systems scenarios | Yes |
| `solution/idioms-patterns` | completed C++ idioms and practical-pattern scenarios | Yes |
| `solution/networking` | completed TCP framing and reliability exercises | Yes |
| `solution/boost-asio` | completed asynchronous lifecycle exercises | Yes |
| `solution/mfc-legacy` | completed UI isolation and modernization exercises | Yes |
| `solution/capstone-control-station` | completed Boost.Asio + MFC integration capstone | Yes |
| `solution/unique-handle` | completed `01_unique_handle` exercise | Yes |
| `solution/bounded-queue` | completed `02_bounded_queue` exercise | Yes |
| `answers` | integration of all reviewed solution branches | Yes |

后续模块继续使用 `solution/<module-name>`。不要把 solution branch merge 回 `main`。

当前仓库若存在未提交 learner work，不要直接切换或创建 solution branch。先把 scaffold 安全提交到 `main`，再把
learner diff 单独迁移到对应 solution branch；迁移前始终检查 `git diff` 和 `git status`。

## Start a module

先确保题目脚手架已经在 `main`，且当前 learner work 已提交到正确分支。然后：

```sh
git switch main
git switch -c solution/algorithms
```

只完成该模块的 TODO、quiz answers 和必要测试调整。每个 commit 保持小而聚焦，例如：

```text
solve STL invalidation exercise
solve lower-bound boundary exercise
solve top-k heap exercise
```

如果多个模块需要同时练习，使用 Git worktree 可以避免频繁 stash，也能防止未提交内容跨分支：

```sh
git worktree add ../control-algorithms solution/algorithms
git worktree add ../control-foundations solution/foundations
```

## Integrate reviewed answers

`answers` 从 `main` 建立一次，然后只接收已 review 的 solution branches：

```sh
git switch main
git switch -c answers
git merge --no-ff solution/foundations
git merge --no-ff solution/algorithms
```

之后新增模块时切到 `answers` 再 merge 对应 solution branch。若 `answers` 已存在，不要再次创建。

## Updating exercises after branches diverge

当 `main` 新增题目或测试后，把更新带入目标 solution branch：

```sh
git switch solution/algorithms
git merge main
```

解决冲突时保留 `main` 的新 tests，同时保留 solution branch 的实现。不要反向把 solution branch merge 到
`main`。最终在 `answers` 上运行完整测试，`main` 上则验证所有 TODO scaffolds 可以构建并按约定 skip/fail。

## Before every commit

1. 用 `git branch --show-current` 确认当前分支。
2. 用 `git diff --staged` 检查是否混入其他模块。
3. 若在 `main`，确认没有完成 TODO、打开 `exercise_complete` 或加入 quiz answer key。
4. 在 solution branch 运行该模块 tests；在 `answers` 运行完整 test suite。
