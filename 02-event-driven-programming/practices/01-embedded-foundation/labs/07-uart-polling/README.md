# Lab 07 - UART Polling

Project standalone có Startup Code, Linker Script, GPIO, UART và `main.c`.

## Chức năng

- USART1 TX trên PA9.
- USART1 RX trên PA10.
- 9600 baud, 8-N-1.
- Poll `TXE` và `RXNE`.
- Echo ký tự nhận được.
- Toggle LED khi nhận byte.

## Build

```bash
cd labs/07-uart-polling
make
make flash-stlink
```

Hoặc:

```bash
make lab07
```
