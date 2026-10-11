# Borrowed lifetime：对象、storage 与观察者如何联系

## 1. Address 只说明位置

Storage 是可以放置对象的内存；object lifetime 是某个对象何时在其中存在。
Scope 是名字可见的范围。这三件事经常一起变化，但并不等价。

一个 local string 离开 scope 时被销毁。Stack 上的字节可能暂时没有覆盖，但那个 string 的 lifetime 已结束。
对残留字节的观察不能代替语言允许的对象访问；不能靠“看起来还能打印”证明引用安全。

## 2. Owner 与 borrower 各自保存什么

Owner 负责让资源在需要期间存在并最终清理。Borrower 只表达访问关系，不承担延长 lifetime 的责任。
Reference/raw pointer 通常借用对象；string_view/span 描述一段非拥有的数据。

```cpp
std::string owner = "sensor";
std::string_view view = owner;
```

view 保存访问字符范围所需的信息，不建立独立字符副本。
按值复制或返回 view，只复制这个描述；underlying characters 仍由原 owner 管理。
所以“按值返回”要继续问：返回的 value 是 owning data，还是 borrowing descriptor？

## 3. Borrowing contract 有两个轴

第一是 owner lifetime：数据拥有者是否仍存在。
第二是 invalidation：owner 还活着，但它是否替换、移动或释放了原 storage。

例如 string 的操作若重新分配字符 storage，旧 view 不会自动跟随新地址。
vector reallocation 同样让旧元素的 pointers/references 失效。具体哪些操作失效，要看容器的 contract。

还有内容有效性：复用同一 frame buffer 可能没有释放 storage，因此 view 不 dangling，
但它观察到的是下一帧，而不是之前的帧。Memory safety 与业务数据正确性仍是不同问题。

## 4. Temporary lifetime extension 为什么不是传染性的

```cpp
const std::string& local = std::string{"sensor"};
```

这类直接 local binding 会把 temporary 的 lifetime 延长到该 reference 的 lifetime 结束。
这是特定初始化规则，不是所有 reference 都自动保活。

返回 local object 的 reference，或在 return statement 中返回 temporary 的 reference，
不会把它的 lifetime 延长到 caller。Caller 再绑定一个 reference 也不会复活已销毁对象。
某些参数绑定 temporary 只活到包含调用的 full-expression 结束；保存引用到更晚仍会悬空。

思考方法是追踪原对象在哪个规则下创建、何时销毁，不能只看最后一个变量的 type。

## 5. 返回接口如何表达 lifetime

Owning string return 把独立内容交给 caller，便于长期保存；copy elision/move 常能减少转交成本。
Borrowed view return 可以减少复制，但调用方要知道 owner 与 invalidation 边界。

指向 string literal 的 view 可以有效，因为 literal 有 static storage duration。
指向 local string 内容的 view 在函数返回后失效。它们的 return type 相同，安全性来自不同的 storage lifetime。

API 设计时应明确 view 有效到下一次 mutation、到 owner destruction，还是某个显式 operation boundary。
如果调用方很容易跨过这个边界，owning result 可能比注释更可靠。

## 6. Async 把 borrowing contract 变得更长

Callback 可能在当前函数返回很久之后执行；借用 local buffer 通常无法跨越这一时间。
可以复制所需数据、捕获 owner 保活，或把 callback 执行限制在明确有效的时间内。

捕获 owner 也只解决 lifetime：如果另一个线程修改同一 buffer，仍需同步；
如果 buffer 被复用，仍需保证内容语义。Ownership、invalidation、concurrency 必须一起说明。

## 理解检查与实践入口

为一个借用值写出四件事：它观察谁、谁 owns、何时销毁、哪些操作会失效或改变内容。
再分析同一个 view 被同步使用与被 callback 保存时，contract 为什么不同。

理解后做 [move_lifetime exercise](../exercises/move_lifetime/README.md)。不要解引用 dangling pointer
作为测试；sanitizer 可提供诊断，但没有报告不等于 lifetime 已证明正确。

> A borrowed value describes access without owning the data. Its validity depends on both the owner's lifetime and invalidation rules. Returning a view by value does not make its underlying storage independent.
