# MFC Boundary and Legacy Strategy

MFC window/control objects belong to the UI thread。worker/Asio thread 不应直接调用 controls；它应产生 immutable view
model，通过 dispatcher/PostMessage 交给 UI thread。message handler 只做 conversion 和 delegation，业务规则位于 pure
C++ service。

window 可能在 queued callback 执行前销毁，因此 callback 不能无条件捕获裸 view pointer。可使用 subscription token、
weak lifetime guard 或在 destroy sequence 中先 detach dispatcher。legacy modernization 顺序是 characterization tests →
extract seam → move one behavior → compare results，而不是一次重写。
