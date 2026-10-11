# Shared ownership：对象为什么能活到最后一个使用者结束

## 1. 从一个 owner 到多个 owner

Unique ownership 把清理责任放在一个对象上。Shared ownership 允许多个独立的持有者共同决定 lifetime：
只要还有 strong owner，managed object 就继续存在；最后一个 strong owner 消失时，执行销毁策略。

这需要共享一份 lifetime bookkeeping，称为 control block。
shared_ptr 的 mental model 是 stored pointer 加 ownership identity；前者决定 get() 访问哪里，
后者决定与谁共同管理 lifetime。不要把 shared_ptr 理解成“带计数的 raw pointer 地址”。

## 2. Control block 如何工作

Control block 保存 strong/weak bookkeeping 及销毁 managed object 所需的信息，例如 deleter。
复制已有 shared_ptr 加入同一 ownership group；销毁或 reset 一个 owner 离开该 group。

```text
owner A ─┐
owner B ─┼── control block ── managed object
weak W ──┘   observes ownership, does not keep object alive
```

最后的 strong owner 消失时销毁 object。Weak observations 可以让 bookkeeping 继续存在，
直到它们也消失。make_shared 常采用合并分配，所以对象 lifetime 结束后，相关分配的 storage 可能仍保留。
“对象已经销毁”与“内存分配已释放”不一定发生在同一时刻。

weak_ptr::lock 尝试取得同组的 strong owner；成功后局部 shared_ptr 保活，失败返回 empty。
不能先 expired() 再用 raw pointer，因为检查与使用之间 lifetime 可能变化。

## 3. 为什么相同地址不合并 ownership

Raw pointer 只提供地址，没有可查证的 ownership identity。
从它独立构造 shared_ptr，相当于建立新的 ownership group，并指定自己的清理责任。
如果两个 groups 用默认删除器管理同一对象，双方都认为自己有最终 delete 的权利。
Reference count 本身没有错，错的是出现了两份不协调的账本。

相反，aliasing constructor 可以共享 control block，却用不同 stored pointers 指向 managed object 的成员。
成员本身不是分别删除的对象；owner group 保持 enclosing object 的 lifetime。

C++20 可以用 `!a.owner_before(b) && !b.owner_before(a)` 判断相同 ownership ordering identity。
get() 相等不能替代它；empty owners 也可以同组等价，结果不证明有一个活对象。
use_count() 也是某一 group 的瞬时计数，不是业务同步工具。

## 4. shared_from_this 如何找到已有账本

对象的成员函数有 this，但 this 只有地址。enable_shared_from_this 提供关联已有 ownership group 的机制，
可以理解为对象内有一个由 shared_ptr 建立关联的 weak observation。

类型需要 public、无歧义地继承相应的 enable_shared_from_this<T>，对象也必须先由合适的 shared_ptr 管理。
shared_from_this 从这一关联取得 strong owner；关联不存在时会抛 bad_weak_ptr。
shared_ptr<T>(this) 则从地址新建一组 ownership，绕开已有账本。

普通 make_shared 的 constructor 执行期间，关联尚未完成，所以需要 constructor 之后的 start/factory 阶段。
weak_from_this 在无关联时返回 empty，不抛这个异常；此时保存的 empty weak_ptr 不会稍后自动接上。

## 5. Async callback 把 ownership 跨越时间传递

同步调用时，调用方常能保证对象在调用期间 alive。异步排队后，调用方可能已经销毁对象。
Callback 的 capture 必须表达 operation 对 lifetime 的需求：

- strong capture 保存 owner，让 object 活到 capture 被释放。Callback 被丢弃时也会释放，不需要先执行。
- weak capture 只观察；执行时 lock，成功才使用，失败就跳过。
- raw this capture 不保活，需要另一份可证明的 lifetime contract。

强引用只保证对象仍存在，不保证业务仍允许工作。Shutdown 后 callback 仍可能到达；
需要 operation state、cancellation policy 与同步机制决定是否继续处理。

## 6. 为什么 cycle 与 data race 仍然存在

对象拥有 stored callback，callback 拥有 strong self，就构成环。
外部 owners 全消失，环内计数仍非零，所以没有最后一个 owner，destructor 也无法启动破环。
解决方式是改变 ownership graph，例如 weak capture，或明确可靠地清除 callback。

同组的不同 shared_ptr objects 可以并发管理 ownership，但 managed object 的字段不会自动受到保护。
同一个 shared_ptr variable 的并发读写也需要对应同步或 atomic shared_ptr。
Lifetime、同步和业务 cancellation 是三种独立责任。

## 理解检查与实践入口

画出 strong/weak owners、control block、managed object，并沿着 reset 推导销毁时机。
再给 callback 添加 shutdown 状态，说明“alive”为什么不代表“仍可执行任务”。

理解后做 [shared_ownership exercise](../exercises/shared_ownership/README.md)，用 fake executor 显式推进
callback 检查生命周期。不实际运行 double-delete 的代码来证明风险。

> Shared ownership coordinates lifetime through a control block. shared_from_this joins an existing ownership group. Strong captures keep an object alive, while weak captures allow it to disappear; synchronization and cancellation still need separate contracts.
