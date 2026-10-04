# C++ Idioms and Practical Patterns

## Part I — C++ idioms（重点）

### RAII

constructor 建立资源 ownership/invariant，destructor 做 non-throwing cleanup。适用于 memory、socket、file、lock、thread、
transaction、registration token。RAII 的价值是所有 scope-exit 路径都 cleanup，而不只是正常 return。

### Rule of Zero / Five

业务类型优先 Rule of Zero，让 `vector`、`string`、`unique_ptr` 等成员管理资源。直接 owning resource 的低层类型才通常
需要明确 destructor、copy/move constructor 和 copy/move assignment。不要机械地“定义了一个就全部手写”；先决定
copyable、move-only 还是 immovable 的语义。

### Copy-and-swap

把可能失败的 copy 放在 temporary，使用 `noexcept swap` commit，temporary 清理旧状态。它提供清晰的 strong guarantee，
但可能牺牲 storage reuse。不是所有 assignment 都应使用它。

### Move-and-reset / exchange

move-only owner 将 resource 转移，并把 source 置为明确的 empty/valid state。`std::exchange` 常能清楚表达该动作。仍需考虑
self-move、aliasing 和 destination 旧资源的释放。

### Scope guard / rollback guard

默认在 scope exit 执行 rollback；显式 success/commit 后 dismiss。destructor 不应 commit，因为 stack unwinding 时无法可靠
区分业务操作是否成功，且 destructor 不能安全传播 commit failure。

### PImpl

public class 只保存指向 implementation 的 smart pointer，把 platform/framework headers 移到 `.cpp`。收益是减少编译耦合、
隐藏实现和稳定 ABI；代价是 allocation、indirection、copy semantics 与 incomplete-type destructor 需要仔细设计。

### Type erasure

`std::function` 等把 concrete callable type 隐藏在统一 value-like interface 后。适合 callback/plugin seam，但有 allocation、
indirection、copyability 和 lifetime 成本。第一轮只需会使用和解释，不手写完整 type-erasure framework。

### Non-copyable movable owner

资源只能有一个 owner 时删除 copy，提供 `noexcept` move。典型例子是 socket、file handle、thread 和 transaction guard。

### `enable_shared_from_this`

异步对象需要共享 lifetime 时，从 existing control block 取得 `shared_ptr`；不能用 `shared_ptr(this)` 新建 control block。
是否捕获 strong self 或 weak self 取决于 operation 是否应延长 lifetime。

### Erase-remove / algorithm-first

使用标准算法表达 intent，理解 logical end 与 container erase。它同时是 STL idiom 与 invalidation 练习。

## Part II — Practical patterns（少而精）

### Strategy

将变化的 policy 放在 interface/function object 后，例如 retry、interlock、filter 或 scheduling policy。只有确实需要 runtime
替换时才引入 virtual interface；compile-time policy 或普通 callable 可能更简单。

### State

connection、alarm、device mode 和 shutdown 适合显式 state + allowed transitions。它减少散落 booleans，但不要为两个简单
状态制造复杂 class hierarchy；enum + transition function 往往足够。

### Adapter

把 Boost.Asio、MFC、Win32 或 legacy API 转换成 core 所需的小接口。adapter 负责 translation，不应吸收 domain logic。

### Observer

适用于 telemetry/UI notification。关键风险是 subscriber lifetime、unsubscribe、reentrancy、thread affinity 和 slow consumer。
value snapshot + dispatcher 通常比裸 observer pointer 更安全。

### Command

把 operator action 表示成 value：包含 identity、parameters、timestamp/idempotency key，可 validation、queue、audit 和 retry。
不要把 arbitrary closure 当作无法检查的业务 command。

### Factory

集中创建 platform-specific implementation 或维护 construction invariant。若只是调用一个 constructor，不必额外制造 factory class。

### Dependency injection

这是设计原则而非必须使用 framework。constructor 参数接收小 interface/reference/value policy 通常足够，能让 core 用 fake
transport/clock/store 测试。

## Patterns to treat carefully

- Singleton：隐藏 global state、lifetime/order 和 test isolation 问题；
- deep inheritance：coupling、fragile base、ownership 不清；
- Visitor：封闭 type set 有用，但会提高结构复杂度；
- double-checked locking：不要手写，优先 local static 或 `call_once`；
- copy-on-write：mutation、threading 和 reference invalidation 语义复杂；
- “manager”/“service locator”：常掩盖依赖和 ownership。

## Decision questions

选择 idiom/pattern 前问：

1. 谁 owns resource，对象何时销毁？
2. 哪个 operation 可能失败，需要什么 exception guarantee？
3. 哪个变化轴需要隔离：platform、policy、state 还是 presentation？
4. 是否真的需要 runtime polymorphism？
5. callback/thread boundary 的 lifetime 与 synchronization 是什么？
6. 更简单的 value type、free function 或 composition 是否足够？

Senior-level 重点不是说出 pattern 名，而是解释它解决的问题、代价和不用它时的替代方案。
