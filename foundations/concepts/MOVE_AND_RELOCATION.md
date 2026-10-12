# Move 与 relocation：表达式怎样变成资源转移

## 1. 对象、表达式和构造是三层东西

对象是实际存在的实体；表达式指称对象或产生值；constructor 根据表达式建立新对象。
把这三层分开，才能解释为什么 std::move 自己不移动资源。

```cpp
T source;
T destination(std::move(source));
```

source 是一个 object。std::move(source) 产生指称同一对象的 xvalue expression。
随后 overload resolution 选择可用的 constructor；被选中的 constructor 才决定复制还是转移资源。
原对象仍然存在，std::move 没有销毁它，也没有创建第二个 owner。

## 2. 为什么需要允许 move 的表达式

普通 named variable expression 是 lvalue：调用方通常还会继续使用它。
如果 library 随意接管它的资源，程序很难推理。Rvalue expression 给接口一个机会：调用方允许使用可消耗的路径。
具体能否消费、消费后状态如何，仍由 type contract 决定。

T&& 是 rvalue reference type，但有名字的 reference variable 在表达式中仍是 lvalue：

```cpp
auto&& reference = std::move(source);
T another(reference); // reference is a named lvalue expression
```

这解释了为什么接收 T&& 的函数在继续转交该参数时常需要 std::move。
const 也不会因 std::move 消失；const T&& 通常无法绑定常见的 T(T&&)，可能改用 copy。

## 3. Move 为什么通常更便宜，但并非总是

Deep copy 为 destination 建立独立资源，例如分配并复制 buffer。
Move 可以交接 buffer ownership，然后把 source 留在可接受的状态，不必复制每个字节。

但语言不保证 move 是 O(1)，也不保证不抛异常。某些 type 的 move 仍需要 allocation 或维护复杂关系。
Moved-from 的可用操作由 contract 决定；标准库类型通常 valid but unspecified，不能普遍断言 empty。
自定义类型也需要明确自己的 invariant 和 moved-from contract。

## 4. Container relocation 的真正问题

vector 的元素存在连续 storage 中。容量不足时，需要新 storage，把元素构造过去，再销毁旧元素、释放旧 storage。
这是对象 lifetime 和 ownership 的变化，不只是复制几个地址。

若 copy 第 k 个元素失败，之前构造好的新元素可以销毁；旧元素通常尚未改变。
若 move 第 k 个元素失败，前面的源元素可能已经被消费，第 k 个源元素也可能已改变。
此时 catch 并不能恢复原来的旧 vector；旧内容可能已经不存在。

所以异常保证影响的是操作开始前的策略：copy 可以保留旧状态，non-throwing move 可以排除移动中的失败点。
Throwing move-only type 无法同时拥有这两个条件；必须接受相关操作的较弱保证。
例如 reserve 在非 CopyInsertable 元素的 move 抛异常时允许 effects unspecified。

## 5. move_if_noexcept 是选择引用的工具

| Nothrow move constructible | Copy constructible | 返回引用 |
|---|---|---|
| true | 任意 | T&& |
| false | true | const T& |
| false | false | T&& |

```cpp
T destination(std::move_if_noexcept(source));
```

函数根据 compile-time traits 返回引用，不分配、不 deep copy、不尝试 move 后 retry。
随后 destination 的 initialization 才调用 constructor。
vector 可以采用这个思路，但标准不要求内部必须调用这个函数。

Traits 判断 expression 是否可构造、是否可能抛异常，不是检查有没有名字叫 move constructor 的函数。
只有 noexcept copy constructor 的类型，也可能满足 is_nothrow_move_constructible：rvalue 可以绑定 const T&。
因此 T&& 结果也不证明最终发生了资源转移。

## 6. noexcept 为什么影响系统行为

一个可信的 noexcept move contract 让 container 可以先做可能失败的 preparation，再用不会失败的 moves 迁移。
这与 exception safety 的 prepare/commit 模型相连。

为了让 move 更可靠，可以把失败的初始化和设备工作放在独立操作中，让 ownership transfer 只交接已有资源。
不能为了让 vector 选择 move 就给一个会抛异常的实现贴 noexcept；违反承诺会 terminate。

## 理解检查与实践入口

按顺序解释 expression category → overload selection → constructor → source state → cleanup。
再画 vector 新旧 storage 的元素 lifetime，标出 copy 和 move 各自的失败点。

理解后做 [move_lifetime exercise](../exercises/move_lifetime/README.md)，用 traits 和构造计数验证选择，
不用“运行后碰巧是空的”推导普遍 contract。

> std::move changes the expression category; a selected constructor performs the actual operation. During relocation, copying can preserve the old state, while a non-throwing move removes a failure point. move_if_noexcept selects a reference using type traits.
