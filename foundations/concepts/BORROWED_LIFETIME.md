# Borrowed lifetime：有地址不代表有对象

## 概念与 trade-offs

reference、raw pointer、string_view、span 通常借用已有对象或 storage。
返回一个 view 的值，只复制 view 的描述信息，不复制它观察的内容。

```cpp
std::string value = "sensor";
std::string_view view = value;
```

value 销毁后 view 悬空；value 仍存在时，重新分配字符 storage 也可能让 view 失效。
这分别是 owner lifetime 和 invalidation 问题。残留字节不是活着的对象，也不是可安全读取的证据。

local `const std::string& r = std::string{"sensor"};` 直接绑定 temporary，temporary 的 lifetime
延长到该 local reference 的 lifetime 结束。函数返回 reference 并不把这种延长传给 caller。
绑定函数返回的 dangling reference 不能复活原对象。

按值返回 string 通常表达 owning result，容易使用；返回 view 避免复制，但调用方必须遵守 owner lifetime
与 invalidation contract。返回 string literal 的 string_view 可以有效，因为 literal 有 static storage duration。
不要因此推出所有按值返回的 view 都安全。

在实时视频 pipeline 中，借用 frame buffer 传给异步 callback 前，必须确认 buffer 是否会被复用或销毁。
必要时捕获 owner、复制数据或约束 callback 在 buffer 有效期内完成。这与 NDK/JNI 的边界生命周期讨论相通。

## 用自己的话复述

分别解释 owner 销毁、storage reallocation、buffer 内容被复用，会如何影响借用者。
最后一种可能不是 dangling，却仍可能读到错误帧。

## Runnable exercise 与验收

完成 [move_lifetime](../exercises/move_lifetime/README.md)，使用其已有验收测试。

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build -R foundation_move_lifetime_exercise --output-on-failure
```

Review 重点：返回对象是否 owns 内容？借用 contract 是否明确？不要解引用 dangling pointer 验证结果。
sanitizer 可辅助发现问题，但没有报错不能证明 lifetime 正确。TODO 状态的 SKIPPED 不等于完成。

## Interview follow-up

一个 getter 返回内部 string_view。调用方把它缓存到下一次 update 之后，你会怎样改善 API contract？

## Concise English interview answer

> A view returned by value still borrows its underlying storage. I check both owner lifetime and invalidation rules. For independent results I prefer an owning return value; for views I document how long the storage remains valid.
