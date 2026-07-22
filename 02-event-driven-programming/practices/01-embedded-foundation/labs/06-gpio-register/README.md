# Lab 06 - GPIO Register-Level

Project standalone có đầy đủ Startup Code, Linker Script, driver GPIO và `main.c`.

## Mục tiêu

- Bật clock GPIOC.
- Cấu hình PC13 output push-pull 2 MHz.
- Điều khiển LED active-low bằng `GPIOC_BSRR`.

## Build

```bash
cd labs/06-gpio-register
make
make flash-stlink
```

Hoặc:

```bash
make lab06
```
