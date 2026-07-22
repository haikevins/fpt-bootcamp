# Lab 02 - Memory Access

## Mục tiêu

- Ép địa chỉ số thành con trỏ.
- Đọc và ghi dữ liệu qua dereference.
- Hiểu vai trò của `volatile`.
- Hiểu rủi ro của địa chỉ không hợp lệ.

## API

```c
uint32_t memory_read32(uintptr_t address);
void memory_write32(uintptr_t address, uint32_t value);
```

Lab chạy trên host 64-bit nên dùng `uintptr_t`. Trên Cortex-M3, không gian địa chỉ là 32-bit.

## Build và chạy

```bash
make lab02
./build/labs/lab02-memory-access
```

## An toàn

Chương trình chỉ truy cập địa chỉ của một biến hợp lệ. Không thay địa chỉ bằng số ngẫu nhiên. Truy cập sai có thể gây segmentation fault trên host hoặc HardFault/BusFault trên MCU.

## Liên hệ Memory-Mapped I/O

```c
#define GPIOC_ODR \
    (*(volatile uint32_t *)0x4001100CU)
```

Nguyên lý con trỏ giống nhau, nhưng địa chỉ được ánh xạ tới peripheral thay vì SRAM.
