# Networking and TCP Framing

顺序：[`NETWORKING.md`](NETWORKING.md) → [`EXERCISES.md`](EXERCISES.md) → frame decoder TODO → tests。

第一阶段只覆盖笔试高频核心：TCP 是 byte stream、消息 framing、partial/multiple reads、length validation、buffer
growth、disconnect/timeout/retry。TLS、DNS 深入实现和具体工业协议留到笔试后。

```sh
cmake --build build --target frame_decoder_exercise_tests
ctest --test-dir build -R networking_ --output-on-failure
```

答案放在 `solution/networking`，不进入 `main`。
