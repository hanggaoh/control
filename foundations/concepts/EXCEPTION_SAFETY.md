# Exception safety：程序在失败时如何保持可理解的状态

## 1. 一个操作会改变什么

把程序看成一组活着的 objects，以及它们拥有的 resources。对象的 state 是成员当前的值；resource
可以是内存、文件、socket 或锁。两者有联系，但不是同一件事：释放 buffer 可以消除泄漏，却不能恢复
一个已经改掉的配置值。

对象还需要 invariant：在对外可用的时刻必须成立的关系。例如 buffer 的长度等于 size，或者一个有效
handle 恰好由一个 owner 负责关闭。成员各自合法不够，成员之间的关系也必须合法。

一次 operation 可以分成很多步骤：分配、构造、修改成员、发送请求、释放旧资源。
Exception safety 研究的是：如果其中某一步失败，控制流离开后，留下的对象和资源是什么状态。
它不是要求程序永远不失败，而是要求失败的后果有明确 contract。

## 2. throw 改变控制流，但不倒放程序

```cpp
state.mode = Mode::active;
perform_work(); // may throw
state.ready = true;
```

如果 perform_work 抛出异常且这里没有处理它，最后一行不会执行。已经完成的 mode assignment 不会自动撤销。
throw 开始寻找匹配的 handler；沿调用链离开作用域时，执行 stack unwinding。

Unwinding 会销毁离开作用域的、已完成构造的 automatic objects，通常按构造的逆序销毁。
它不会对任意成员写入、磁盘写入或网络请求生成 rollback log。catch 得到的是处理失败的机会，
不是“世界已经回到函数调用前”的证明。

若没有匹配 handler，最终调用 terminate；无 handler 的情形下，不应依赖完整 unwinding 必然发生。
下面的清理推理针对异常传播到可处理边界的正常路径。

## 3. RAII 为什么能处理资源清理

```cpp
void process() {
    auto buffer = std::make_unique<Buffer>();
    perform_work(); // may throw
}
```

buffer 的 ownership 记录在一个活着的对象中。正常离开 scope 或异常导致 unwinding 时，
unique_ptr 的 destructor 都执行对应清理。机制成立的原因是“ownership 与 object lifetime 绑定”，
不是因为 exception 特别理解 buffer。

如果用 raw pointer 保存 new 的结果，pointer 自身销毁只丢弃地址，不会 delete 指向的对象。
资源必须及时进入 owner；owner 建立之前发生的失败不能指望它帮忙。

Constructor 抛异常时，完整对象没有成功构造，因此不会调用完整对象的 destructor；
已经构造完成的 bases 和 members 会被销毁。这就是资源应优先放在 RAII members 中的原因。
Destructor 若在 unwinding 中又让异常逃出，会 terminate，所以清理路径应避免抛出。
关闭设备失败可以记录或由显式 close 接口报告，但不能把 destructor 当作常规恢复入口。

## 4. Guarantees 是失败后的状态 contract

| Guarantee | 失败后允许观察到什么 |
|---|---|
| Basic | 无资源泄漏，invariant 成立，但值可能改变 |
| Strong | 对约定保护的状态没有效果，失败相当于该操作未发生 |
| No-throw | 异常不会逃出该操作 |

Basic 与 strong 描述失败后状态；no-throw 描述异常传播。不要把它们机械地理解成运行速度或业务成功率等级。
例如一个 noexcept 函数可以用 error code 报告业务失败。

保证必须有范围：内存配置保持原样，不等于已发送的网络命令没有效果。
日志、计数器等副作用是否纳入 contract，也应明确。Strong guarantee 通常不是整个世界的事务。

## 5. Strong guarantee 从哪里来

要保留旧状态，只有几种基本思路：在所有可能失败的工作完成前不修改它；或者保留足够的信息，
在失败后执行可靠的 rollback。后者要求 rollback 本身可靠，并覆盖全部相关修改，因此往往更难推理。

更直接的结构是：

```text
                 prepare fails
old state ── prepare ───────────────> old state remains
                 succeeds
                     ↓
             candidate state
                     ↓ non-throwing commit
                  new state
                     ↓ cleanup old resources
```

Prepare 在独立的候选对象里做 allocation 和 construction。失败时候选对象的 RAII members 清理，
目标仍是旧状态。Commit 只执行不会失败的 ownership/state 交接。随后清理旧资源。
这是 strong guarantee 的结构来源。

## 6. copy-and-swap 为什么符合这个结构

copy-and-swap 先复制候选对象，再交换 target 与 candidate 的状态。
成功交换后，target 拥有新状态，candidate 拥有旧状态；candidate 的 destructor 清理旧资源。

关键是各阶段的性质，而不是 swap 这个名字：copy 必须不修改 target；完整 commit 必须不抛异常；
cleanup 不能抛出。如果交换 payload 后、交换 size 前抛异常，target 可能混合了新旧状态，
甚至违反 payload.size() == size。Temporary 析构只能清理它此时持有的资源，不能修复这种关系。

swap 可以含多个操作；只要没有中途异常逃出，异常安全推理仍可成立。但这不代表线程间 atomic：
并发观察者仍可能看到中间状态，需另外用同步保护。

## 7. noexcept 怎样参与设计

noexcept 是“异常不会逃出”的语言 contract，compiler/library 可以据此选择操作。
若实际有异常逃出，程序 terminate；它不会自动补充 catch 或 rollback。

正确顺序是先证明实现及其调用的操作不抛异常，再声明 noexcept。资源 owner 的 swap 通常只交换
handle/pointer 和配套 metadata，避免在 commit 中分配内存或做设备 I/O。

copy-and-swap 的成本是候选副本及额外 allocation，可能无法复用已有容量。
需要优化时可以换算法，但仍要逐个标出 failure point 和状态变化，重新证明 contract。

## 8. 放回完整系统

RAII 解决 resource lifetime；invariant 定义合法 state；exception guarantee 定义失败后 state；
noexcept 帮助建立可靠的 commit/cleanup 边界；error handling boundary 决定记录、重试或向上报告。

设备命令一旦发出，可能出现“设备执行了，但响应丢失”。内存 swap 无法撤销这个副作用。
这时需要 protocol 层的幂等、状态查询或补偿策略，不能只依赖 C++ exception safety。

继续阅读：[move 与 relocation](MOVE_AND_RELOCATION.md)，理解 library 如何利用这些保证选择操作。

## 理解检查与实践入口

用自己的话描述一次 throw 后：哪些语句不再执行、哪些对象被销毁、哪些修改仍保留。
再解释为什么“没有 leak”与“旧状态不变”是两个独立结论。

理解机制后做 [copy_swap exercise](../exercises/copy_swap/README.md)；用 allocation failure injection
检查 target 的 invariant 与旧值。练习实现仍由你完成。

> Exception safety is a contract about the state left after failure. Stack unwinding runs destructors, so RAII cleans up resources, but it does not undo arbitrary mutations. A strong guarantee usually comes from preparing independently and committing without throwing.
