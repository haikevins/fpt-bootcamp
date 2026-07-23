# Lab 05 - Linker Script

Project standalone gồm `main.c`, `startup.c`, `memory.ld` và `Makefile`.

## Nội dung khảo sát

| Object | Section |
|---|---|
| `g_firmware_version` | `.rodata` |
| `g_initialized_counter` | `.data` |
| `g_uninitialized_counter` | `.bss` |
| `g_work_buffer` | `.bss` |
| `g_reset_record` | `.noinit` |

## Build

```bash
cd labs/05-linker-script
make
make report
```


## Phân tích

```bash
arm-none-eabi-objdump -h build/lab05_linker_script.elf
arm-none-eabi-nm -n build/lab05_linker_script.elf
less build/lab05_linker_script.map
```
