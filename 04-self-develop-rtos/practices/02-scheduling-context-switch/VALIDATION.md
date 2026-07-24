# Validation Report

## Automated checks performed

The repository was validated in the generation environment with:

```bash
python3 tools/check_structure.py
./tools/check_all.sh
```

## Results

- Repository structure check: PASS
- Root Scheduler Playground cross-build with Clang/LLD: PASS
- Root ELF size: 4,820 bytes text, 0 bytes data, 5,064 bytes BSS
- Lab 01 Cortex-M3 target cross-build: PASS
- Lab 02 initial-stack host tests with ASan/UBSan: PASS
- Lab 02 host demo: PASS
- Lab 03 SVC first-task target cross-build: PASS
- Lab 04 cooperative PendSV target cross-build: PASS
- Lab 05 priority-policy host tests with ASan/UBSan: PASS
- Lab 05 host demo: PASS
- Lab 06 fixed-priority target cross-build: PASS
- Lab 07 SysTick/time-slice target cross-build: PASS
- Lab 08 EXTI preemption target cross-build: PASS
- Lab 09 round-robin target cross-build: PASS
- Lab 10 context-switch stress target cross-build: PASS
- Root Makefile contains no lab build rules: PASS
- All 10 labs contain their own local Makefile: PASS

## Not validated in this environment

The firmware was not flashed to a physical STM32F103C8T6. Hardware validation
is still required for:

- SVC first-task exception return on the target;
- PSP/MSP behavior observed through GDB;
- PendSV register preservation over long stress runs;
- SysTick frequency at 1 kHz;
- EXTI0 preemption using PA0;
- PC13 LED behavior;
- USART1 output on PA9/PA10 at 9600 baud;
- stack guard and high-water results after long execution.
