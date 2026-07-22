# 01 — Embedded Foundation

Repository thực hành cho **Chủ đề 1: Kiến thức nền tảng trong Embedded System Programming**.

Project được viết theo hướng **bare-metal, register-level**, không sử dụng HAL hoặc SPL. Cấu hình mặc định:

- **Board:** STM32F103C8T6 Blue Pill.
- **CPU:** ARM Cortex-M3.
- **Flash:** 64 KiB.
- **SRAM:** 20 KiB.
- **Clock:** HSI 8 MHz sau reset.
- **LED:** PC13, active-low.
- **UART:** USART1, PA9/PA10, 9600 baud, 8-N-1.

> Khi dùng board/MCU khác, cần sửa `linker/memory.ld`, địa chỉ thanh ghi, chân LED, chân UART và tần số clock.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Giải thích địa chỉ, dữ liệu, con trỏ và endianness.
- Phân biệt Flash, SRAM, peripheral memory và system region.
- Phân biệt `.isr_vector`, `.text`, `.rodata`, `.data` và `.bss`.
- Hiểu luồng reset từ Vector Table đến `main()`.
- Tự viết Startup Code và Linker Script tối thiểu.
- Điều khiển GPIO bằng thanh ghi.
- Gửi/nhận UART bằng polling.
- Tạo `.elf`, `.bin`, `.hex`, `.map` và `.lst`.
- Phân tích firmware bằng `size`, `nm`, `objdump` và map file.

---

## 2. Cấu trúc repository

```text
01-embedded-foundation/
├── README.md
├── Makefile
├── .gitignore
├── linker/
│   └── memory.ld
├── startup/
│   └── startup.c
├── src/
│   ├── main.c
│   ├── gpio.c
│   └── uart.c
├── include/
│   ├── gpio.h
│   └── uart.h
├── labs/
│   ├── 01-endianness/
│   ├── 02-memory-access/
│   ├── 03-memory-sections/
│   ├── 04-startup-code/
│   ├── 05-linker-script/
│   ├── 06-gpio-register/
│   ├── 07-uart-polling/
│   └── 08-map-analysis/
├── docs/
│   ├── memory-map.md
│   ├── startup-flow.md
│   ├── sections-analysis.md
│   └── map-analysis.md
└── build/
```

- Phần root là firmware tổng hợp cuối Chủ đề 1.
- `labs/` trình bày từng bài thực hành.
- `docs/` chứa tài liệu phân tích sâu hơn.
- `build/` chỉ chứa artifact sinh tự động.

---

## 3. Luồng firmware

```text
Reset
  |
  v
Vector Table
  |
  v
Reset_Handler
  |
  +--> Copy .data từ Flash sang SRAM
  |
  +--> Clear .bss
  |
  v
main()
  |
  +--> Khởi tạo LED PC13
  |
  +--> Khởi tạo USART1
  |
  +--> Gửi banner
  |
  v
Super-loop
  |
  +--> Toggle LED
  |
  +--> Poll UART RX và echo ký tự
```

---

## 4. Cài đặt toolchain

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    make
```

Công cụ flash tùy chọn:

```bash
sudo apt install openocd stlink-tools
```

Kiểm tra:

```bash
arm-none-eabi-gcc --version
arm-none-eabi-objcopy --version
arm-none-eabi-size --version
make --version
```

---

## 5. Build firmware

```bash
make
```

Artifact:

```text
build/embedded_foundation.elf
build/embedded_foundation.bin
build/embedded_foundation.hex
build/embedded_foundation.map
build/embedded_foundation.lst
```

Các lệnh khác:

```bash
make size
make sections
make symbols
make symbols-size
make disasm
make clean
make rebuild
```

---

## 6. Flash firmware

Dùng ST-Link tools:

```bash
make flash-stlink
```

Dùng OpenOCD:

```bash
make flash-openocd
```

Mass erase:

```bash
make erase
```

---

## 7. UART

Kết nối:

| STM32F103 | USB-UART |
|---|---|
| PA9 — TX | RX |
| PA10 — RX | TX |
| GND | GND |

Mở terminal:

```bash
picocom -b 9600 /dev/ttyUSB0
```

Kết quả dự kiến:

```text
STM32F103 bare-metal foundation
Clock: HSI 8 MHz
USART1: 9600 8-N-1
Type characters to test echo.
```

Firmware sẽ echo ký tự nhận được.

---

## 8. Build các lab chạy trên host

```bash
make lab01
make lab02
make lab03
```

Hoặc:

```bash
make host-labs
```

Chạy:

```bash
./build/labs/lab01-endianness
./build/labs/lab02-memory-access
./build/labs/lab03-memory-sections
```

---

## 9. Danh sách bài thực hành

| Bài | Chủ đề | Kết quả chính |
|---:|---|---|
| 1 | Endianness | Quan sát thứ tự byte của `0x12345678` |
| 2 | Memory access | Đọc/ghi dữ liệu thông qua địa chỉ |
| 3 | Memory sections | Phân tích `.rodata`, `.data`, `.bss` |
| 4 | Startup Code | Vector Table, copy `.data`, clear `.bss` |
| 5 | Linker Script | Bố trí Flash và SRAM |
| 6 | GPIO register-level | Điều khiển LED PC13 |
| 7 | UART polling | TX, RX và echo USART1 |
| 8 | Map analysis | Phân tích Flash/RAM và symbol |

---

## 10. Ghi chú kỹ thuật

### LED PC13 active-low

```text
PC13 = 0 -> LED sáng
PC13 = 1 -> LED tắt
```

### Clock

Startup Code chưa cấu hình PLL. MCU dùng HSI 8 MHz; USART1 vì vậy được tính với PCLK2 bằng 8 MHz.

### Runtime

Firmware root link bằng `-nostdlib` và `-nostartfiles`. Không sử dụng startup mặc định, `printf()` hoặc heap.

### Build directory

`build/` không được commit, ngoại trừ `.gitkeep`.

---
