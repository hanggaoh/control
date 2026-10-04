# MFC and Legacy Modernization

当前 MinGW 环境不提供 MFC；主分支因此只编译 portable presenter/dispatcher seam。真正 `CWnd`、`PostMessage` 和
message-map adapter 在 MSVC Windows 的 `solution/mfc-legacy` 实现，不让 MFC types 进入 domain core。

顺序：[`MFC_LEGACY.md`](MFC_LEGACY.md) → [`EXERCISES.md`](EXERCISES.md) → UI lifetime TODO → tests。

系统设计关联：阅读 [`Capstone system design`](../capstone/SYSTEM_DESIGN.md)，再完成
[`90-minute design TODO`](../capstone/SYSTEM_DESIGN_TODO.md) 中的 UI/core boundary、snapshot、subscriber lifetime 和关闭窗口部分。

```sh
cmake --build build --target mfc_ui_boundary_exercise_tests
ctest --test-dir build -R mfc_ --output-on-failure
```
