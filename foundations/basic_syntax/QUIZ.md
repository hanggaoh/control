# Basic Syntax Quiz

每组先答判断/选择，再做简答。答案放 solution branch。

## 1. Initialization and deduction

- 选择：`const int n=3; auto a=n; auto& b=n;`，`a/b` 分别是什么类型？
- 判断：`decltype(n)` 与 `decltype((n))` 一定相同。
- 简答：initialization 与 assignment 的区别？`int x;` 和 `int x{};` 在函数局部有什么区别？

## 2. Pointer/reference

- 判断：`int& r=x; r=y;` 会将 r 重新绑定到 y。
- 选择：`const int*` 与 `int* const` 各限制对象还是 pointer？
- 简答：设计只读参数、可修改参数、optional observation，分别选择什么类型及 lifetime contract？

## 3. Functions

- 判断：两个函数只要 return type 不同，就可以 overload。
- 选择：const object 能否调用只有 non-const overload 的 member function？
- 简答：为什么看见 function declaration 仍可能 link error？默认参数由什么位置决定？

## 4. Classes

- 判断：member 初始化顺序由 initialization list 的书写顺序决定。
- 选择：`explicit Name(std::string)` 能否直接用于 `Name n{std::string{"x"}}`？
- 简答：instance member 和 static member 的 storage/lifetime 有什么区别？

## 5. Value categories

```cpp
void inspect(const std::string&) {} // A
void inspect(std::string&&) {}      // B
std::string name="sensor";
const std::string fixed="pump";
inspect(name);              // 1
inspect(std::move(name));   // 2
inspect(fixed);             // 3
inspect(std::move(fixed));  // 4
```

- 选择：四次分别选 A 还是 B？
- 判断：`std::move(name)` 自身会转移资源；调用空函数 B 后 name 一定为空。
- 简答：为什么 named `T&&` expression 是 lvalue？forwarding reference 与普通 rvalue reference 有何区别？

## 6. Polymorphism

- 判断：通过没有 virtual destructor 的 base pointer 删除 derived object 总是安全。
- 选择：把 derived object 按值传给 base parameter，可能发生什么？
- 简答：`override` 帮助 compiler 检查什么？composition 与 inheritance 如何选择？

## 7. Templates/build

- 判断：concept 检查通过就能证明算法具有正确 complexity 和 thread safety。
- 选择：template definition 通常放 header 的原因是优化、instantiation visibility，还是所有函数都必须放 header？
- 简答：compile error、link error、runtime lifetime error 各给一个例子；inline 怎样连接 ODR？
