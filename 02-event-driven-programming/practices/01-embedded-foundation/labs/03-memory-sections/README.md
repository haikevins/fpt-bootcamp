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
make lab03
./build/labs/lab03-memory-sections
```

## Phân tích ELF host

```bash
nm -S --size-sort build/labs/lab03-memory-sections
objdump -h build/labs/lab03-memory-sections
readelf -S build/labs/lab03-memory-sections
```

## Phân tích firmware ARM

```bash
make
arm-none-eabi-nm -S --size-sort build/embedded_foundation.elf
arm-none-eabi-objdump -h build/embedded_foundation.elf
```
