# UI dispatcher lifetime TODO

补全 presenter：worker-side snapshot 必须通过 dispatcher 排队；queued callback 只持有 weak view lifetime，view 已关闭时
安全 no-op。测试验证调用发生在 drain 时而不是 producer 调用栈中。
