# Validation Report

## Makefile ownership

- Root `Makefile`: final RTOS Memory Explorer project only.
- Each `labs/<lab>/Makefile`: that lab only.
- No root rule invokes, builds, tests or cleans a lab.

## Automated validation commands

### Final project

```bash
make clean
make TOOLCHAIN=clang
```

### STM32 labs

```bash
make -C labs/01-memory-layout clean
make -C labs/01-memory-layout TOOLCHAIN=clang

make -C labs/02-startup-sections clean
make -C labs/02-startup-sections TOOLCHAIN=clang

make -C labs/10-target-demo clean
make -C labs/10-target-demo TOOLCHAIN=clang
```

### Host labs

```bash
make -C labs/03-stack-high-water clean all
make -C labs/04-static-memory-budget clean all
make -C labs/05-first-fit-basic clean all
make -C labs/06-block-splitting clean all
make -C labs/07-block-coalescing clean all
make -C labs/08-invalid-free clean all
make -C labs/09-fragmentation clean all
```

## Results

- Root Memory Explorer firmware: PASS
- Lab 01 STM32 cross-build: PASS
- Lab 02 STM32 cross-build: PASS
- Lab 03 host tests and demo build: PASS
- Lab 04 host demo build: PASS
- Lab 05 host tests and demo build: PASS
- Lab 06 host tests and demo build: PASS
- Lab 07 host tests and demo build: PASS
- Lab 08 host tests and demo build: PASS
- Lab 09 statistics/randomized tests and demo build: PASS
- Lab 10 STM32 cross-build: PASS
- AddressSanitizer: PASS for host allocator labs
- UndefinedBehaviorSanitizer: PASS for host allocator labs

## Hardware validation still required

The firmware was not flashed to a physical STM32F103C8T6 in this environment. Verify:

- PC13 active-low LED behavior;
- USART1 PA9/PA10 at 9600 baud;
- HSI 8 MHz assumption;
- `.data` and `.bss` result in Lab 02;
- allocator output and validator result in Lab 10;
- Memory Explorer UART commands on the target board.
