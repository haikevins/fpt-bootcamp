# Lab 03 - Memory Sections

## Mục tiêu

Khảo sát `.rodata`, `.data`, `.bss` và `.text`.

| Biến | Section dự kiến |
|---|---|
| `g_firmware_version` | `.rodata` |
| `g_initialized_counter` | `.data` |
| `g_uninitialized_counter` | `.bss` |
| `g_static_buffer` | `.bss` |
| `g_private_value` | `.data` |

## Build và chạy

```bash
cd labs/03-memory-sections
make
make run
```

## Phân tích ELF host

```bash
make symbols
make sections
make elf-sections
make report
```

Các báo cáo được tạo trong `build/` của Lab 03.

## Phân tích firmware ARM

```bash
make
arm-none-eabi-nm -S --size-sort build/embedded_foundation.elf
arm-none-eabi-objdump -h build/embedded_foundation.elf
```
