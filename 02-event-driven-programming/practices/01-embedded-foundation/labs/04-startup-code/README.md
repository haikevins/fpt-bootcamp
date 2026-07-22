# Lab 04 — Startup Code

Project standalone:

```text
04-startup-code/
├── README.md
├── Makefile
├── memory.ld
├── startup.c
└── main.c
```

## Mục tiêu

- Tạo Vector Table.
- Thiết lập Initial Main Stack Pointer.
- Đi vào `Reset_Handler`.
- Copy `.data` từ Flash sang SRAM.
- Clear `.bss`.
- Gọi `main()`.

`main.c` kiểm tra trực tiếp biến `.data` và `.bss`.

- LED PC13 nháy chậm: Startup Code hoạt động đúng.
- LED PC13 nháy nhanh: kiểm tra `.data` hoặc `.bss` thất bại.

## Build

```bash
cd labs/04-startup-code
make
```

Hoặc từ root:

```bash
make lab04
```
