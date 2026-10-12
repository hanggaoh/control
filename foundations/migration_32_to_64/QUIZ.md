# 32-bit to 64-bit Migration Quiz

先口头回答，并说明 failure mode 与验证方法。

1. 为什么 Windows x64 上不能假设 `long` 是 64 位？LLP64 分别让哪些类型变宽？
2. 为什么把所有 `int` 全部替换成 `std::size_t` 会引入新的 signed/unsigned 和 sentinel bug？
3. 什么情况下可以使用 `std::uintptr_t`？为什么保留 pointer 类型通常更好？
4. `std::vector<T>::size()` 赋给 `int` 有什么风险？你会如何处理 API boundary 的 narrowing？
5. 为什么 raw struct serialization 可能在 x86 可用、在 x64 失效？
6. fixed-width integer 是否已经足以定义稳定协议？还需要明确哪些 contract？
7. throwing conversion 或 allocation 发生后，迁移代码如何维持原有 exception guarantee？
8. 为什么 x86 executable 不能直接加载 x64 DLL？你会如何盘点完整 dependency chain？
9. 64 位程序为什么可能消耗更多 memory，甚至因为 cache locality 变差而变慢？
10. 如何设计 x86/x64 双构建、compatibility tests、分阶段发布和 rollback？

## Interview follow-up

下面代码在 32 位系统中看似工作，迁移后有哪些问题？

```cpp
unsigned long token = reinterpret_cast<unsigned long>(pointer);
int length = payload.size();
std::printf("%u", payload.size());
send(socket, reinterpret_cast<const char*>(&message), sizeof(message), 0);
```

