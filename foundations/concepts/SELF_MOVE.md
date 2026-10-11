# Move assignment：两个名字可能指向同一个 owner

## 1. Ownership 是责任，不只是地址

Unique resource owner 表示一个 invariant：每个有效资源恰好有一个负责清理的 owner。
Handle 是用于访问资源的值；复制 handle 的整数或 pointer，不会自动复制资源，也不会建立安全的 ownership。

Move construction 建立一个新对象的 ownership。Move assignment 面对一个已经存在的 destination，
它可能已经拥有旧资源，因此需要同时处理“旧责任结束”和“新责任开始”。

## 2. 两个不同对象时发生什么

```text
before: destination owns A, source owns B
after:  destination owns B, source has no ownership; A has been released
```

实现中必须确保 B 不会泄漏或被重复释放，A 不会被遗忘。
清空 source 的成员是为了让它的 destructor 不再关闭 B，不是销毁 source object。
Source 的 lifetime 仍继续，且应符合该类型约定的 moved-from invariant。

释放、取得 handle、清空 source 的具体顺序，要根据是否可能失败及中间状态证明。
一个 helper 能简化某个步骤，却不能独立证明整个 assignment 正确。

## 3. Self-move 改变了对象图

```cpp
x = std::move(x);
```

std::move 没有生成副本。在 assignment body 中，this 与 &other 是同一地址。
之前 destination/source 两个方框，现在只有一个方框；通过任一名字修改成员，另一名字立即看到相同修改。

如果先 release destination，再读取 source，读取的就是刚释放后的自己。
若 release 清空 handle，最终可能为空；若只 close 而未清空，可能重新持有失效 handle。
先复制 handle 再 release source 也不能普遍解决，因为保存的 handle 可能指向刚关闭的同一资源。

根本问题是 aliasing：不能把两个参数名字当作两个独立 object。

## 4. 合法状态与保持原值是不同 contract

一种简单策略是 self-move no-op：识别别名，直接保持原资源。
另一种设计允许 self-move 后为空，但必须无泄漏、无 double release，并保持 invariant。
标准库通常承诺 self-move 后 valid but unspecified，不普遍承诺原值。
自定义 wrapper 可以选择更明确、更强的 contract。

Temporary-and-swap 也可能建立安全的路径，但要整体证明 move construction、swap、cleanup 对别名的行为。
std::exchange 只负责“读旧值、写新值”，不会自动识别整段算法的 aliasing。

## 5. Resource lifetime 与 error policy

Move assignment 往往希望 noexcept，避免在 ownership 交接中暴露半完成状态。
这要求 cleanup 和 transfer 的实际行为支持这个承诺。

若 close 会报告失败，应明确失败意味着资源仍存在、已关闭，还是状态未知；不能随意 retry 一个可能被
系统复用的 handle。Destructor 没有方便的错误返回通道，通常应采用非抛出清理和单独的可观察错误策略。
Noexcept 不解决这些业务语义，只禁止异常逃出。

## 6. 放回完整系统

容器移动元素、return owning values、把 socket 交给 session，都依赖可靠的 move contract。
可靠的 resource wrapper 让上层只考虑谁 owns，而不重复编写 close 顺序。
Self-move 是检验这个 ownership 模型在 aliasing 下是否仍成立的一种边界情况。

## 理解检查与实践入口

分别画 distinct-object move 与 self-move 的 ownership 图，逐步标出每次释放的资源。
说明“valid”“empty”“unchanged”三种承诺的差异。

理解后做 [unique_resource exercise](../exercises/unique_resource/README.md)，按其 contract 用 fake closer
检查每个有效 handle 的释放次数，覆盖空 owner、非空 owner、reset 与 self-move。

> Move assignment replaces ownership in an existing object. Self-move makes source and destination aliases, so the transfer must preserve the invariant under that condition. I define the self-move contract explicitly and verify resource cleanup exactly once.
