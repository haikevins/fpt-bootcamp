# RTOS Introduction

RTOS tập trung vào khả năng đáp ứng deadline có thể dự đoán, không chỉ tốc độ trung
bình. Các khái niệm nền tảng:

- latency;
- response time;
- deadline;
- jitter;
- WCET;
- determinism;
- hard, firm và soft real-time.

Một RTOS cung cấp task, scheduler, context switch, blocking, IPC, synchronization và
timer. Việc sử dụng RTOS không tự bảo đảm firmware đạt real-time; priority, blocking,
critical section và worst-case behavior vẫn phải được phân tích.
