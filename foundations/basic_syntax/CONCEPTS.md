# Basic Syntax Concepts

## 1. Initialization, auto and decltype

initialization 建立对象，assignment 修改已有对象。`int n;` 的局部 scalar 未初始化；`int n{};` 为零。
brace initialization 拒绝部分 narrowing，但 initializer-list overload 可能改变选择。
`auto` 按值推导通常丢弃 reference 和顶层 const；`auto&` 保留引用关系。
`decltype(name)` 取得实体声明类型，`decltype((name))` 根据表达式 value category 推导，named variable expression 是 lvalue。

## 2. Pointer, reference and parameter passing

pointer 可为空、可重新指向；reference 必须初始化且不能重新绑定。两者都不自动拥有资源。
按值参数得到独立对象；`T&` 可以修改 caller；`const T&` 只读借用，需要有效 lifetime。
`const T*` 限制通过 pointer 修改对象；`T* const` 限制 pointer 自身重新赋值。

## 3. Functions and overloads

overload resolution 根据可用 candidates 与 conversion rank 选函数；仅 return type 不构成 overload。
默认参数在 call site 使用，不改变函数参数类型。`const` member function 可以用于 const object，不能普通修改非 mutable members。
声明、定义和链接是不同阶段：可见 declaration 不保证最终存在 definition。

## 4. Classes and special members

member initialization list 在 constructor body 之前执行，实际顺序由 member declaration 决定。
`explicit` 阻止对应 constructor 被用于普通 implicit conversion。destructor 管 cleanup。
`static` member 属于 class，不属于某个 instance；C++17 `inline static` data member 可在 header 定义。
资源语义连接到 Rule of Zero/Five，不要把所有类都写成 resource owner。

## 5. Value categories and move

named variable expression 是 lvalue，即使变量类型是 `T&&`。`std::move` 是产生 xvalue 的 cast，本身不搬资源。
是否发生资源转移取决于后续 constructor/assignment/function 的行为。对 const 对象 move 通常得到 `const T&&`，不能绑定普通 `T&&`。
`T&&` 在 deduced template parameter 场景可以是 forwarding reference；reference collapsing 中出现 `&` 时结果通常为 `&`，
只有 `&&` 与 `&&` 合并得到 `&&`。使用 `std::forward<T>` 保留传入 category。

## 6. Inheritance and virtual dispatch

`virtual` 支持通过 base reference/pointer dispatch 到 derived implementation；`override` 检查确实覆盖 base function。
按 base value 复制 derived object 会 slicing。通过 base pointer delete derived object，需要适当 virtual destructor。
interface contract 同时包含 lifetime、errors 和 thread behavior，连接到 SOLID/LSP。

## 7. Templates, concepts and ODR

template definition 通常需要在 instantiation point 可见；concept 约束可接受类型，不自动证明业务语义。
header 可定义 templates 和合规 inline entities；普通 non-inline function definition 放进多个 translation units 可能违反 ODR。
`inline` 的语言意义包括允许符合规则的多重定义，不保证 optimizer 展开 call。
