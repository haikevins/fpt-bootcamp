# Target validation checklist

- [ ] Flash thành công qua ST-Link/OpenOCD.
- [ ] UART PA9/PA10 hiển thị banner ở 9600 baud.
- [ ] Task đầu tiên chạy bằng PSP.
- [ ] SVC/PendSV/SysTick chạy bằng MSP.
- [ ] LED PC13 đổi trạng thái theo high task.
- [ ] `s` in scheduler statistics.
- [ ] `t` in trace records.
- [ ] Worker A/B được round-robin.
- [ ] Stack guards còn nguyên sau stress run.
- [ ] Không HardFault.
