# Incremental frame decoder TODO

补全 `frame_decoder::feed`：支持 fragmented header/payload、一次输入多个 frames、zero-length payload，并在读取 payload
或分配前拒绝超过 `max_payload` 的 length。进入 error state 后不再产出 frames。

完成后将 `exercise_complete` 改为 `true`，运行 `networking_frame_decoder_exercise` tests。
