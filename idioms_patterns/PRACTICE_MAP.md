# Practice Map

本模块不复制代码题，而是复用已有练习：

| Idiom / pattern | Existing practice | Review focus |
|---|---|---|
| RAII + move-only owner | `exercises/01_unique_handle` | exactly-once release、self-move、`noexcept` move |
| Rule of Five / copy-and-swap | `foundations/exercises/copy_swap` | prepare/commit/cleanup、strong guarantee |
| move selection | `foundations/exercises/move_lifetime` | `move_if_noexcept`、container relocation |
| shared async lifetime | `foundations/exercises/shared_ownership` | control block、strong/weak capture |
| scope/rollback guard | `database/exercises/transaction_guard` | destructor rollback、explicit commit |
| algorithm-first | `algorithms/exercises/stl_safety` | erase-remove、invalidation |
| Strategy | `control_systems` | filter/interlock/retry policy boundary |
| State | `networking`, `boost_asio`, `capstone` | connection/generation/shutdown transitions |
| Adapter | `mfc_legacy`, `capstone` | framework types stay outside core |
| Observer | `mfc_legacy`, `capstone` | UI dispatch、subscriber lifetime |
| Command | `capstone` | validation、queue、audit、idempotency |
| Dependency injection | networking/MFC seams | fake clock/transport/dispatcher |

每次 review 只回答四个问题：problem、ownership/lifetime、trade-off、simpler alternative。
