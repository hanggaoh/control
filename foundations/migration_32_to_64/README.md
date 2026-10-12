# 32-bit to 64-bit C++ Migration

目标不是机械地把类型变大，而是找出旧代码对 pointer width、integer width、object layout 和
binary interface 的隐含假设。Windows x64 使用 **LLP64**：pointer 和 `std::size_t` 是 64 位，
但 `int` 和 `long` 仍然是 32 位。

## Review map

1. **Pointer truncation**：不要把 pointer 存入 `int`、`unsigned long` 或 `DWORD`。确需整数表示时，
   使用 `std::uintptr_t`；如果不需要数值运算，就保留 pointer 类型。
2. **Size and index**：容器大小和内存大小通常使用 `std::size_t`，地址差使用
   `std::ptrdiff_t`。不要把所有 `int` 全局替换为无符号类型。
3. **Windows pointer-sized types**：审查 `INT_PTR`、`UINT_PTR`、`LONG_PTR`、`ULONG_PTR`、
   `DWORD_PTR`、`WPARAM`、`LPARAM`、`LRESULT`，以及 `SetWindowLongPtr` 一类 API。
4. **Layout and ABI**：pointer alignment 变化会改变 `sizeof`、padding 和 field offset。
   DLL、COM、plugin、callback 与 shared-memory boundary 必须单独审查。
5. **Persistence and protocols**：不要把 raw struct、`size_t`、pointer 或未固定底层类型的 enum
   直接写入文件或网络。使用固定宽度字段，明确 byte order、version 和 validation。
6. **Formatting and varargs**：`std::size_t` 使用 `%zu`，pointer 使用 `%p`。重点检查 logging
   和自定义 variadic functions。
7. **Dependencies and operations**：x64 process 只能加载 x64 library。检查 third-party binaries、
   installer、registry/WOW64 behavior、memory usage、cache locality 和 rollback path。

## Migration workflow

1. 建立可重复的 x86 baseline 和 characterization tests。
2. 开启严格 warnings，盘点 narrowing conversions、casts、ABI 和 persisted formats。
3. 建立 x64 build，同时保留 x86 CI。
4. 按 boundary 分类修复，避免全局搜索替换类型。
5. 做 cross-architecture file/network/IPC compatibility tests。
6. 做 memory、performance、long-running 和 failure-path tests。
7. 分阶段发布，保留 observability 与 rollback。

## Coding exercise

只修改 `include/migration_32_to_64.hpp` 中的 TODO：

- 用能够无损表示 pointer 的标准整数类型保存地址值。
- 把进程内的 `std::size_t` 长度安全地转换为 32-bit protocol length；越界时返回空值。
- 让 wire header 的字段宽度不依赖目标 architecture。

完成后把对应的 `*_complete` flag 改成 `true`：

```sh
cmake --build build --target migration_32_to_64_exercise_tests
ctest --test-dir build -R foundation_migration_32_to_64 --output-on-failure --verbose
```

先完成 [`QUIZ.md`](QUIZ.md)，再写代码。不要用 `sizeof(struct)` 作为 wire format。
