# STM32F103C8T6 Memory Map

```text
Flash: 0x08000000, 64 KiB
SRAM:  0x20000000, 20 KiB
```

Repository reserve 1 KiB ở cuối SRAM cho main stack. Linker kiểm tra `.data`, `.bss`
và `.noinit` không va chạm vùng này.

Các section chính:

- `.isr_vector`: vector table;
- `.text`: code;
- `.rodata`: constants;
- `.data`: initialized writable globals;
- `.bss`: zero-initialized globals;
- `.noinit`: dữ liệu không được startup clear;
- main stack: vùng `[0x20004C00, 0x20005000)`.
