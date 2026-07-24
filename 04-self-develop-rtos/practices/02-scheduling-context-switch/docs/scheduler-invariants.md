# Scheduler invariants

- READY/RUNNING task thuộc đúng một ready queue.
- BLOCKED task không thuộc ready queue.
- Ready bitmap khớp với trạng thái queue.
- Current task là head của highest-priority ready queue.
- Idle task luôn READY.
- `saved_sp` nằm trong stack range và stack guard còn nguyên.
- Chỉ scheduler helper được thay đổi state/list membership.
