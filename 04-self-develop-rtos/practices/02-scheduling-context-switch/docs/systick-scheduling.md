# SysTick scheduling

SysTick chạy 1 kHz từ HSI 8 MHz. Handler tăng kernel tick, release task delay,
giảm time slice và chỉ pend PendSV khi cần. SysTick không trực tiếp restore
context task khác.
