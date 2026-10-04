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

## SOLID in C++ — 30–45 Minute Review

SOLID 用于检查 component boundaries 和替换 contract。C++ 可以通过 composition、virtual interfaces、templates 或
function objects 实现这些原则；选择取决于实际变化需求、ownership 和测试方式。

| Principle | C++ interpretation | Capstone application |
|---|---|---|
| S — Single Responsibility | 围绕一个变化原因组织相关逻辑，保持 cohesive invariant | parser、session、repository、MFC presenter 分工 |
| O — Open/Closed | 已知变化轴通过 policy/interface 扩展，稳定流程保持集中 | 替换 retry/interlock policy |
| L — Liskov Substitution | 替换实现仍满足调用方依赖的完整 contract | fake/real transport 的 callback、error、cancel 语义一致 |
| I — Interface Segregation | 按调用方需求提供小接口 | telemetry reader、command sender、audit writer |
| D — Dependency Inversion | 核心依赖稳定 contract，adapter 实现该 contract | StationService 注入 transport、repository、dispatcher |

### SRP and OCP: choose real change boundaries

SRP 不意味着每个 method 都拆一个 class。将一起维护 invariant 的 state 和操作放在一起，再隔离 protocol、persistence、
UI 等独立变化原因。OCP 针对已知变化需求；新增 policy 后应能验证原流程，而不是为了未知需求预先建立大量接口。

### LSP: signatures are only the beginning

实现不能加强调用前置条件、削弱结果保证，或破坏 invariant。在 C++ 中还需要核对：

- 谁拥有参数/result，reference/view 的有效期是什么；
- exception guarantee 和 `noexcept` 承诺是否保持；
- callback 在什么 executor 上执行，是否 inline、是否允许 reentrancy；
- accepted operation 的 completion 次数和 cancellation 后行为；
- shutdown 后是否仍会投递 callback，何时可以销毁对象。

例如 contract 要求 callback 异步投递到指定 executor，fake transport 却立即在 caller thread 调用，会破坏线程和
reentrancy 假设。测试替身同样需要遵守 contract；compiler 只能检查其中一部分。

### ISP and DIP: make dependencies explicit

避免让只读 telemetry consumer 依赖包含 write、admin、connection control 的巨大接口。按 client needs 拆分，同时保持
可理解的职责。DIP 可以用 constructor 注入 `ITransport&`，也可以使用 template/callable policy；runtime substitution
更适合 virtual interface，固定策略可以用 compile-time composition。

非 owning reference 要求 dependency 比 service 活得更久；`unique_ptr` 表示转移独占 ownership。引入抽象后仍必须说明这些
lifetime contracts。通过 base pointer 删除对象时，base destructor 需要支持安全的 polymorphic destruction。

### Review checklist

1. 一个组件包含哪些独立变化原因？哪些状态必须共同维护 invariant？
2. 是否存在真实 policy variation，最小扩展点是什么？
3. fake 与 real implementation 是否遵守相同 lifetime/error/thread/cancellation contract？
4. 每个 client 是否依赖不需要的能力？
5. core 是否可以通过注入 fake 测试，dependency ownership 是否明确？

将本节与 [`EXERCISES.md`](EXERCISES.md) 的 SOLID quiz 和 Capstone audit 一起完成。

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
