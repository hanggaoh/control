# Traditional C++ → Modern C++ 小结

这里的 “Modern C++” 通常指 C++11 之后逐步形成的风格，而不是“所有旧写法都错误”。核心变化是：
让 ownership、lifetime、type constraints 和 failure 更明确，让 compiler 与 library 承担更多正确性工作。

## 现代 C++ 符号解码地图

Modern C++ 的新符号多数不是增加新的底层机器能力，而是把旧代码中依赖注释和人工检查的意图交给
type system 与 compiler。先按它们解决的问题分类，比逐个死记更快：

| 目的 | 常见写法 | 直觉 |
|---|---|---|
| lifetime / ownership | `&`、`&&`、`std::move`、smart pointer、RAII | 借用、交接、自动释放 |
| 检查程序员意图 | `override`、`explicit`、`= delete`、`[[nodiscard]]` | 把 code-review 规则变成 compile check |
| 自动生成代码 | `= default`、`auto`、template、concept | 让 compiler 推导或生成 boilerplate |
| operation contract | `const`、`noexcept`、`constexpr` | 是否修改、是否抛异常、能否编译期计算 |
| 编译前文本处理 | `#include`、`#ifndef`、`#define` | preprocessor 先组合 source text |

### `[[...]]`：attribute

双中括号是 attribute，给 compiler 额外信息：

```cpp
[[nodiscard]] int open_device();
[[maybe_unused]] int debug_value = 0;
```

`[[nodiscard]]` 表示返回值不应被随意忽略，违反时通常产生 warning。它不是 array，也不是返回 type。
其他常见标准 attributes 有 `[[deprecated]]`、`[[maybe_unused]]`、`[[fallthrough]]`。

### `&`、`&&` 与 `std::move`

```cpp
void inspect(const Widget& value); // read-only borrow
void consume(Widget&& value);      // may consume/move from value
```

- `T&`：借用一个已有 object。
- `const T&`：只读借用，也可绑定 temporary。
- `T&&`：rvalue reference，通常表示对象即将销毁或允许资源被接管。
- `std::move(x)`：本身不移动资源，只把 `x` cast 成“允许被 move”的表达式；真正的转移发生在随后调用的
  move constructor 或 move assignment 中。

类比：`&` 是借阅证，`&&` 是交接单，`std::move` 是在交接单上签字。move 后的原对象仍必须 valid，
但内容通常 unspecified，适合销毁或重新赋值。type 中的 `&&` 不要与 boolean `a && b` 混淆。

### `operator...` 与 `override` 完全不同

`operator` 定义 type 对现有运算符的行为，叫 operator overloading：

```cpp
Widget& operator=(const Widget& other); // a = b
int& operator[](std::size_t index);     // a[index]
explicit operator bool() const;         // convert to bool
```

它们仍是 function，只是有特殊名字。`a = b` 可理解为 `a.operator=(b)`。

`override` 用于 inheritance，要求 member function 确实覆盖 base 的 virtual function：

```cpp
struct Base {
    virtual void stop() = 0;
    virtual ~Base() = default;
};

struct Device final : Base {
    void stop() override;
};
```

signature 写错时 compiler 会报错，避免它悄悄变成另一个 overload。`final` 表示 class 不可再继承，或
virtual function 不可再 override。

### `explicit`：不允许 compiler 偷走转换捷径

```cpp
class Seconds {
public:
    explicit Seconds(int value);
};

Seconds timeout{5};  // allowed and clear
// Seconds timeout = 5; // rejected
```

单参数 constructor 可能成为 implicit conversion。`explicit` 要求调用方明确构造，防止 unrelated value
意外变成你的 type。conversion operator 也可以使用 `explicit`。

### `= default` 与 `= delete`

```cpp
Widget() = default;
~Widget() = default;
Widget(const Widget&) = delete;
Widget& operator=(const Widget&) = delete;
```

- `= default`：明确要求 compiler 按成员语义生成 special member function；它不等于随手写一个空 `{}`。
- `= delete`：function 存在于接口和 overload resolution 中，但调用它就是 compile error，常用于禁止 copy。

类比：`default` 是 compiler 标准套餐；`delete` 是明确锁死这扇门。

### `inline`：重点是 ODR，不是强制优化

```cpp
inline int twice(int value) { return value * 2; }
inline constexpr int protocol_version = 2;
```

optimizer 自行决定是否展开 function call。语言层面更重要的是：相同 inline definition 可以出现在多个
translation units 中，linker 将其视为同一 entity。因此 header 中定义的 function/variable 常见 `inline`。

### `template` 与 `concept`

```cpp
template <typename T>
T maximum(T left, T right);

template <std::totally_ordered T>
T maximum(T left, T right);
```

template 是带 type parameter 的代码配方；调用时 compiler 为具体 type 进行 instantiation。concept 描述 `T`
必须具备的能力，类似 compile-time interface，没有 virtual dispatch。

### `#ifndef` / `#define`：preprocessor，不是 C++ statement

```cpp
#ifndef DEVICE_HPP
#define DEVICE_HPP
// header contents
#endif
```

preprocessor 在 type checking 前做文本级条件处理。这个 include guard 防止同一 header 在一个 translation unit
中被重复展开。常见的 `#pragma once` 更短且被主流 compiler 支持，但不属于 ISO C++ 标准。

### 高频写法速查

| 写法 | 一句话解释 |
|---|---|
| `nullptr` | 有明确 pointer type 的空指针，替代 `0` / `NULL` |
| `auto x = value;` | 从 initializer 推导静态 type，不是 dynamic typing |
| `const` | 通过当前接口不能修改；是 API contract 的一部分 |
| `noexcept` | 承诺异常不会逃出；违反会 `std::terminate` |
| `constexpr` | 可以参与 compile-time computation |
| `enum class` | 名字有作用域且不会随意转换成 integer 的 enum |
| `using Name = Type;` | type alias，模板中通常比 `typedef` 好读 |
| `[capture](args) {}` | lambda；capture 决定保存哪些外部状态 |
| `for (auto& x : values)` | range loop；`&` 表示不复制 element |
| `requires` / `concept` | 限制 template argument 必须支持的操作 |

### 从旧式直觉迁移

| 早期 C++ 直觉 | Modern C++ 直觉 |
|---|---|
| `new` 后记住 `delete` | 把 resource 放进 RAII owner，随 scope 自动清理 |
| pointer 就是一段 address | 先区分 owning 与 non-owning |
| copy 就是复制几个成员 | 判断 value semantics、deep copy 或禁止 copy |
| interface 主要靠 inheritance | runtime interface 用 virtual；compile-time interface 用 concept |
| 注释写“不能调用/不要忽略” | 用 `= delete`、`[[nodiscard]]`、`explicit` 检查 |

阅读复杂 declaration 时逐层拆：它是普通 function、special member 还是 template？参数表达 owning、borrow 还是
move？末尾 `const`、`noexcept`、`override` 各自承诺什么？`[[...]]` 又要求 compiler 检查什么？

## 必须分清的术语

### Declaration 与 definition

```cpp
int read_value();             // declaration
int read_value() { return 1; } // definition

extern int retry_count;       // declaration
int retry_count = 3;          // definition + initialization
```

一个 entity 可以被多次 declaration，但通常只能有一个符合 ODR 的 definition。class definition、
inline function 和 template 有专门规则，可出现在多个 translation units，但内容必须一致。

### Initialization 与 assignment

```cpp
std::string name{"pump"}; // initialization: object lifetime begins
name = "valve";           // assignment: object already exists
```

优先直接 initialization。先 default-construct 再 assignment 可能多做工作，也可能暂时产生 invalid state。

常见 initialization forms：

- default initialization：`Widget w;`
- value initialization：`Widget w{};`
- direct/list initialization：`Widget w{arg};`
- copy initialization：`Widget w = arg;`
- member initialization list：`Owner(int n) : member_(n) {}`

注意：brace initialization 能阻止部分 narrowing conversion，但 “always use braces” 也不是绝对规则，
因为 `std::initializer_list` overload 可能改变 overload resolution。

### Template instantiation

```cpp
template <typename T>
T twice(T value) { return value + value; }

auto result = twice(3); // instantiates a specialization similar to twice<int>
```

template declaration 只是 recipe；`instantiation` 产生具体 specialization。编译器通常必须在
instantiation point 看见完整 template definition，所以 template implementation 经常放在 header。

也可以使用 explicit instantiation，把特定类型的 code generation 集中在 `.cpp`，但会限制可用类型并增加维护成本。

## `inline` 到底是什么

传统理解把 `inline` 当作“请求 compiler 展开函数”。现代 compiler 是否 inline call 主要由 optimizer 决定。

语言层面更重要的意义是 ODR：inline function 或 inline variable 可以在多个 translation units 中拥有一致定义。

```cpp
inline int next_id() { /* ... */ }
inline constexpr int protocol_version = 2;
```

class body 内定义的 member function 隐式具有 inline 语义。`inline` 不代表 thread-safe，也不代表 internal linkage。

## Macro 为什么应谨慎

macro 属于 preprocessor 的 token substitution，不理解 type、scope、namespace 或 overload：

```cpp
#define SQUARE(x) x * x // SQUARE(1 + 2) gives the wrong expression
```

Modern C++ 通常使用这些替代：

| Macro 用途 | 更安全的选择 |
|---|---|
| constant | `inline constexpr` variable |
| function-like macro | function / function template / lambda |
| type selection | template / concept / type alias |
| platform feature | macro 仍常用于 conditional compilation |
| source location | `std::source_location` |

macro 仍有合理用途，例如 include guards、platform switches、compiler attributes compatibility 和 test framework DSL；
原则是限制范围，不用 macro 模拟 type-safe language feature。

## 传统写法与现代倾向

| Traditional tendency | Modern C++ tendency | 设计收益 |
|---|---|---|
| `new` / `delete` scattered in code | value semantics, `make_unique`, containers | ownership 清晰，exception-safe cleanup |
| owning raw pointer | `unique_ptr`，必要时 `shared_ptr` | ownership 可从 type 看出 |
| C array + length | `std::array`, `vector`, `span` | bounds/lifetime contract 更清楚 |
| magic integer constants | `enum class`, `constexpr` | type safety |
| `NULL` / `0` | `nullptr` | overload resolution 正确 |
| macro utility | function/template/`constexpr` | scope、type checking、debugging |
| output parameter everywhere | return value, struct, optional/result | API contract 可读 |
| deep inheritance | composition + small interfaces | coupling 更低，更容易测试 |
| manual lock/unlock | `lock_guard`, `unique_lock`, scoped lock | RAII，异常路径安全 |
| thread flag + polling | condition variable / event-driven async | 正确 wakeup，减少 CPU waste |
| callback owns unclear raw pointer | explicit capture + managed lifetime | 避免 use-after-free |
| giant platform-bound class | pure C++ service + thin adapter | MFC/Qt 与业务逻辑解耦 |

## Rule of Zero / Five

- Rule of Zero：让成员类型自己管理资源，业务 class 不写 destructor/copy/move。
- Rule of Five：resource owner 如果必须自定义 destructor，通常也要明确 copy constructor、copy assignment、
  move constructor 和 move assignment。
- move 不只是 performance feature；对 `unique_ptr`、socket wrapper 等类型，它表达 ownership transfer。

## Exception safety 与 copy-and-swap

常见 exception guarantees：

- no-throw guarantee：operation 不会抛异常。
- strong guarantee：失败时 observable state 不变，像 transaction rollback。
- basic guarantee：失败后 invariant 仍成立且没有 leak，但值可能已经改变。

直接 copy assignment 若先删除旧资源、再分配新资源，分配失败时对象已经被破坏。copy-and-swap
把可能抛异常的工作放进 temporary，成功后才 commit：

```cpp
Widget& operator=(const Widget& other) {
    Widget temporary(other); // prepare: may throw; *this is untouched
    swap(*this, temporary);   // commit: must be noexcept
    return *this;             // temporary destroys the old state
}
```

`swap` 必须只交换已经存在的 representation，并标记 `noexcept`；否则 commit 本身可能只做了一半。
这种写法天然处理 copy self-assignment：`x = x` 先复制出独立 temporary，再交换。代价是即使容量可复用，
通常仍会分配；performance-sensitive type 可以写更复杂的 assignment，但必须重新证明 exception guarantee。

copy-and-swap 不是 Rule of Zero 的替代品。首选仍是让 `vector`、`string`、smart pointer 等成员自动管理资源；
只有教学、底层 resource owner 或特殊 representation 才常需要 Rule of Five。若手写 owning raw pointer，需系统考虑
destructor、copy constructor、copy assignment、move constructor、move assignment，而不是只修一个函数。

## `noexcept` move 与 container relocation

`std::vector` 扩容时要把元素搬到新 storage。若 move constructor 可能抛异常，而 copy 可用，implementation
通常会选择 copy 来维持 strong guarantee。`std::move_if_noexcept(x)` 把这项策略直接表达出来：move 是
`noexcept` 或 type 不能 copy 时返回 rvalue，否则返回 `const T&` 促使 copy。

因此 `noexcept` 是 correctness/performance contract，不是装饰。只有操作确实不会抛出时才声明；错误的
`noexcept` 会让异常触发 `std::terminate`。

`std::move_if_noexcept` 不是“先 move，捕获失败后再 copy”。它在 expression 被构造时就根据 type traits
选择返回 `T&&` 还是 `const T&`：

```cpp
template <class T>
constexpr std::conditional_t<
    !std::is_nothrow_move_constructible_v<T> && std::is_copy_constructible_v<T>,
    const T&, T&&>
move_if_noexcept(T& value) noexcept;
```

所以这里没有 runtime rollback 或 retry。若选择了 throwing move 且它抛出，library 不能靠“再 copy 一次”
恢复已经被 move 修改过的多个旧元素。copy 的价值在于 relocation 期间旧 storage 中的元素保持不变，只有
新 storage 全部构造成功后才整体 commit。

## Reference lifetime

reference 不拥有对象，也不会延长一般对象的 lifetime。返回 local variable 的 reference、保存指向已销毁
container element 的 reference，或在 `vector` reallocation 后继续使用旧 reference，都会 dangling。
临时对象绑定到 local `const T&` 时存在特定 lifetime extension，但把这个 reference 再返回出去不会把 extension
继续传递给 caller。Senior-level 判断应从“被引用对象何时销毁/失效”开始，而不是只看 reference 是否 non-null。

```cpp
const std::string& local = std::string{"sensor"}; // lifetime extends to local's scope

const std::string& make_name() {
    return std::string{"sensor"};                  // wrong: dangling on return
}
```

安全修复通常是 return by value；现代 C++ 的 copy elision/move 让这种接口既清楚又高效。若返回 reference 或
`string_view`，被引用的 owner 必须独立存在，并且 API 要说明 invalidation contract。

## Self-move assignment 与 unique ownership

`x = std::move(x)` 虽然不常见，但可能通过 aliasing、generic code 或容器操作间接出现。一个 naive move assignment：

```cpp
release(handle_);
handle_ = other.handle_;
other.handle_ = invalid_handle;
```

在 `this == &other` 时会先释放唯一资源，然后从同一个、已失效的对象读取，最终资源丢失。可采用显式
self-check；也可用先交换/先构造 temporary 再 commit 的设计。无论策略如何，要求是不能 double release，
对象必须保持 valid invariant。是否承诺 self-move 后保留原值是 API 设计选择；练习中的目标是保留原 ownership。

## `shared_ptr` control block 与 ownership identity

`shared_ptr` 的关键不只是 raw address，而是 control block。control block 保存 strong/weak counts 和 deleter。

```cpp
T* raw = new T;
std::shared_ptr<T> first(raw);
std::shared_ptr<T> second(raw); // wrong: a second independent control block
```

两者的 `get()` 相同不代表 shared ownership 相同；它们会各自尝试 delete，造成 double deletion。应 copy 已有
`shared_ptr`，或从一开始用 `make_shared`。可用双向 `owner_before` 比较 ownership identity；C++26 还提供
`owner_equal`。aliasing constructor 可以拥有同一个 control block 却暴露不同 pointer，这也说明 pointer identity
与 ownership identity 是两回事。

`weak_ptr` 观察 control block 而不增加 strong count，适合打破 ownership cycle。需要使用时调用 `lock()`，
得到一个可能为空的 `shared_ptr`，不要先 `expired()` 再假设对象仍存在。

## `enable_shared_from_this` 与 async lifetime

成员函数中的 `std::shared_ptr<T>(this)` 会新建第二个 control block，语义不等同于 `shared_from_this()`，并可能
double delete。继承 `std::enable_shared_from_this<T>` 后，`shared_from_this()` 从已经存在的 shared ownership
取得同一 control block 的新 owner。对象必须先由合适的 `shared_ptr` 管理；否则会抛 `std::bad_weak_ptr`。

异步 callback 常捕获 `self = shared_from_this()`，使对象活到 callback 完成：

```cpp
executor.post([self = shared_from_this()] { self->on_complete(); });
```

这不是要求所有对象都使用 shared ownership。若 operation 不应延长 lifetime，可捕获 `weak_from_this()`，执行时
`lock()` 并在对象已销毁时取消工作。选择 strong 还是 weak capture 应来自明确的 cancellation/ownership policy。

## Weak Points / Mistake Review

下面七项是本轮实际暴露的薄弱点；复习时重点不是背结论，而是能解释失败路径：

| 薄弱点 | 正确 mental model | 对应实践 |
|---|---|---|
| throwing `swap` | prepare 可以失败但不改变 target；commit 必须不抛异常 | `copy_swap` |
| vector relocation | copy 可保留旧 storage；`noexcept` move 才适合无风险 commit | `move_lifetime` |
| return-reference lifetime | local direct binding 可延长；return 不会把延长传给 caller | `move_lifetime` |
| self-move assignment | `this` 与 `other` 可能 alias；release-before-transfer 会丢资源 | `unique_resource` |
| `shared_ptr` control block | 相同 raw pointer 不等于相同 ownership identity | `shared_ownership` |
| `move_if_noexcept` | compile-time/type-trait driven expression selection，不是 runtime retry | `move_lifetime` |
| `shared_from_this` | 复用 existing control block；`shared_ptr(this)` 创建危险的新 block | `shared_ownership` |

## RAII、Boost.Asio 与 MFC 的共同基础

### RAII

依赖 initialization、destruction、scope、move semantics 和 exception guarantees。重点不是“在 destructor 调 close”，
而是让 object invariant 与 resource ownership 始终一致。

### Boost.Asio

核心问题仍然是 lifetime 和 concurrency：handler 捕获了谁、operation 如何取消、timer/socket 由谁拥有、
completion 是否可能在 shutdown 之后到达、strand 保证什么顺序。

### MFC modernization

MFC message map 本身会使用 macro，这属于 framework boundary 的合理历史设计。现代化不等于重写 MFC；常见策略是：

1. 用 characterization tests 固定现有行为。
2. 从 `CDialog`/`CView` 中提取 pure C++ domain/service logic。
3. 使用 interface 隔离 device/network/database dependency。
4. 保留 thin MFC adapter 处理 message、UI state 和 Windows-specific conversion。
5. 小步替换 raw ownership，保持 backward compatibility。

## 高层判断框架

阅读一段 C++ 时依次问：

1. 这里有哪些 object？它们的 lifetime 是什么？
2. 谁 owns resource？type 是否表达了 ownership？
3. invariant 在 initialization 后是否立即成立？
4. copy/move 会发生什么？异常路径会发生什么？
5. 是否跨 thread 或 async boundary？谁保证 synchronization 和 lifetime？
6. platform/framework dependency 能否被 interface 隔离并测试？

## Concise English interview answer

> Modern C++ is less about using newer syntax and more about making ownership,
> lifetime, invariants, and constraints explicit. I prefer value semantics and
> RAII, use templates and concepts for type-safe generic code, limit macros to
> preprocessing boundaries, and isolate platform frameworks such as MFC behind
> testable C++ interfaces.
