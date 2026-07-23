# 01 - RTOS Introduction and Memory Management

Repository thực hành cho **Chủ đề 1** của lộ trình tự phát triển RTOS:

- RTOS là gì và vì sao hệ thống real-time cần tính xác định.
- Memory layout của MCU.
- Static allocation, stack allocation và dynamic allocation.
- Fragmentation và các lỗi bộ nhớ.
- First-fit allocator, block splitting và block coalescing.

Project được tổ chức theo cùng triết lý với `01-embedded-foundation`:

- Phần root là firmware tổng hợp cuối chủ đề.
- `labs/` chứa từng bài thực hành độc lập.
- `docs/` chứa phần giải thích sâu hơn.
- `build/` chỉ chứa artifact sinh tự động.

Cấu hình mặc định:

- Board: STM32F103C8T6 Blue Pill.
- CPU: ARM Cortex-M3.
- Flash: 64 KiB.
- SRAM: 20 KiB.
- Clock: HSI 8 MHz sau reset.
- LED: PC13, active-low.
- UART: USART1, PA9/PA10, 9600 baud, 8-N-1.
- Code style: bare-metal, register-level, không HAL/SPL.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Phân biệt hệ thống chạy nhanh và hệ thống real-time.
- Giải thích latency, deadline, jitter, WCET và determinism.
- Đọc các vùng `.isr_vector`, `.text`, `.rodata`, `.data`, `.bss`, heap và stack.
- Hiểu luồng reset từ Vector Table tới `main()`.
- Tính static RAM budget cho task, TCB, queue, timer và event pool.
- Phân biệt internal fragmentation và external fragmentation.
- Tự xây first-fit allocator cơ bản.
- Thực hiện splitting, coalescing, invalid-free detection và double-free detection.
- Chạy allocator test trên host trước khi chạy trên MCU.
- Phân tích firmware bằng `size`, `nm`, `objdump`, map file và listing file.

---

## 2. Cấu trúc repository

```text
01-rtos-introduction-memory-management/
├── README.md
├── Makefile
├── .gitignore
├── linker/
│   └── memory.ld
├── startup/
│   └── startup.c
├── src/
│   ├── command_parser.c
│   ├── gpio.c
│   ├── heap.c
│   ├── heap_stats.c
│   ├── main.c
│   ├── memory_explorer.c
│   ├── memory_layout.c
│   ├── runtime.c
│   ├── stack_monitor.c
│   ├── systick.c
│   └── uart.c
├── include/
│   ├── command_parser.h
│   ├── compiler.h
│   ├── gpio.h
│   ├── heap.h
│   ├── heap_stats.h
│   ├── memory_explorer.h
│   ├── memory_layout.h
│   ├── stack_monitor.h
│   ├── stm32f1.h
│   ├── systick.h
│   └── uart.h
├── labs/
│   ├── README.md
│   ├── 01-memory-layout
│   ├── 02-startup-sections
│   ├── 03-stack-high-water
│   ├── 04-static-memory-budget
│   ├── 05-first-fit-basic
│   ├── 06-block-splitting
│   ├── 07-block-coalescing
│   ├── 08-invalid-free
│   ├── 09-fragmentation
│   └── 10-target-demo
├── docs/
│   ├── allocation-models.md
│   ├── allocator-design.md
│   ├── memory-map.md
│   ├── rtos-introduction.md
│   └── startup-flow.md
└── build/
```

---

## 3. Firmware root làm gì?

Firmware root là **Memory Explorer** chạy trên STM32F103:

```text
Reset
  |
  v
Vector Table
  |
  v
Reset_Handler
  |-- Copy .data từ Flash sang SRAM
  |-- Clear .bss
  v
main()
  |-- GPIO PC13
  |-- USART1 polling
  |-- Khởi tạo vùng heap tĩnh 4 KiB
  v
UART command loop
  |-- memory
  |-- alloc <bytes>
  |-- free <id>
  |-- blocks
  |-- stats
  |-- validate
  |-- fragment-demo
  v
First-fit allocator
```

Allocator chỉ phục vụ đào tạo. HairRTOS production kernel vẫn nên theo hướng `static-first`.

---

## 4. Cài toolchain

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    make \
    gcc
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

## 5. Build firmware tổng kết

```bash
make
```

Artifact:

```text
build/rtos_memory_foundation.elf
build/rtos_memory_foundation.bin
build/rtos_memory_foundation.hex
build/rtos_memory_foundation.map
build/rtos_memory_foundation.lst
```

Các lệnh phân tích:

```bash
make size
make sections
make symbols
make symbols-size
make disasm
make clean
make rebuild
```

Trong môi trường chỉ có Clang/LLD:

```bash
make TOOLCHAIN=clang
```

---

## 6. Flash firmware

ST-Link tools:

```bash
make flash-stlink
```

OpenOCD:

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

```text
STM32F103       USB-UART
PA9  TX   --->  RX
PA10 RX   <---  TX
GND        ---  GND
```

Mở terminal:

```bash
picocom -b 9600 /dev/ttyUSB0
```

Banner dự kiến:

```text
RTOS Topic 01 - Memory Explorer
Clock: HSI 8 MHz
USART1: 9600 8-N-1
```

Các lệnh:

```text
help
memory
alloc 32
alloc 80
blocks
stats
free 0
validate
fragment-demo
reset
```

---

## 8. Build các lab

Makefile ở thư mục gốc **chỉ quản lý project tổng kết RTOS Memory Explorer**. Nó không gọi hoặc điều khiển Makefile của các lab.

Mỗi lab được build ngay trong thư mục của chính lab đó:

```bash
cd labs/01-memory-layout
make
```

Ví dụ với lab chạy trên host:

```bash
cd labs/05-first-fit-basic
make test
make run
```

Ví dụ với lab chạy trên STM32 bằng Clang/LLD:

```bash
cd labs/10-target-demo
make TOOLCHAIN=clang
```

Để quay lại thư mục gốc:

```bash
cd ../..
```

Quy ước:

- Lab 01, 02 và 10 tự tạo firmware STM32 trong thư mục `build/` của lab đó.
- Lab 03 đến 09 tự build và chạy trực tiếp trên Ubuntu.
- Các lab allocator chạy test với AddressSanitizer và UndefinedBehaviorSanitizer.
- `make clean` tại root chỉ xóa output của project tổng kết.
- `make clean` trong một lab chỉ xóa output của lab đó.

---

## 9. Danh sách bài thực hành

| Bài | Chủ đề | Kết quả chính |
|---:|---|---|
| 1 | MCU memory layout | In địa chỉ section và linker symbol qua UART |
| 2 | Startup sections | Kiểm tra copy `.data` và clear `.bss` |
| 3 | Stack high-water | Fill pattern và đo stack usage |
| 4 | Static memory budget | Tính RAM cho task, TCB, queue, timer và event pool |
| 5 | First-fit basic | Một block free, allocate/free cơ bản |
| 6 | Block splitting | Tách block lớn thành used block và free remainder |
| 7 | Block coalescing | Gộp các free block liền kề |
| 8 | Invalid free | Phát hiện pointer ngoài heap, pointer giữa block và double free |
| 9 | Fragmentation | Thống kê internal/external fragmentation và randomized test |
| 10 | Target demo | Chạy allocator hoàn chỉnh trên STM32F103 |

---

## 10. Quy trình học đề xuất

```text
Đọc README của lab
      |
      v
Build và chạy lab
      |
      v
Trả lời câu hỏi cuối lab
      |
      v
Cố ý tạo một lỗi
      |
      v
Giải thích nguyên nhân
      |
      v
Chuyển sang lab tiếp theo
```

Không nên chỉ chạy code có sẵn. Với allocator, cần tự thay đổi kích thước allocation, thứ tự `free()` và quan sát block map.

---

## 11. Ghi chú kỹ thuật

### LED PC13 active-low

```text
PC13 = 0 -> LED sáng
PC13 = 1 -> LED tắt
```

### Clock

Startup không bật HSE/PLL. MCU giữ HSI 8 MHz sau reset. USART1 được tính với PCLK2 = 8 MHz.

### Runtime

Firmware root link bằng `-nostdlib` và `-nostartfiles`. Project không dùng startup mặc định, `printf()` hoặc C library heap.

### Allocator

First-fit có worst-case search `O(n)` theo số block. Lab này nhằm giúp hiểu memory management, không nhằm thay thế allocator production cho hard real-time path.
