# First-Fit Allocator Design

Block vật lý:

```text
[header | payload][header | payload]...
```

Header lưu payload size, requested size, previous/next, magic và free state.

Allocation:

```text
align -> first-fit search -> split when useful -> mark allocated
```

Free:

```text
range check -> exact block lookup -> detect double free -> mark free
-> merge next -> merge previous
```

Invariants được kiểm tra bằng `heap_validate()`. Allocator không thread-safe và không
được dùng trong ISR, SysTick hoặc context-switch path.
