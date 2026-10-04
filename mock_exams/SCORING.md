# Mock Scoring and Review

总分 100。建议门槛：第一次 65，第二次 75，最终复测 80。

每题按四个维度评分：correctness 50%、edge/failure paths 20%、trade-off 20%、communication 10%。coding 题若存在 UB、
data race、unbounded allocation 或无法停止，最多获得该题一半分数。

考试结束后只做三件事：

1. 不查资料标出 unsure/guessed answers；
2. 对照 concept/tests，把真正错误写入 `MISTAKE_LOG.md`；
3. 48 小时后只重做错题，不整套立即重刷。
