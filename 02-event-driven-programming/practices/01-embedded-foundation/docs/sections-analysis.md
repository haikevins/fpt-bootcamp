# Section Analysis

## Section chính

| Section | Runtime | Load | Nội dung |
|---|---|---|---|
| `.isr_vector` | Flash | Flash | Vector Table |
| `.text` | Flash | Flash | Mã máy |
| `.rodata` | Flash | Flash | Dữ liệu chỉ đọc |
| `.data` | SRAM | Flash | Biến đã khởi tạo |
| `.bss` | SRAM | Không lưu payload | Biến zero-initialized |

## Ví dụ

```c
const uint32_t version = 1U;       /* .rodata */
uint32_t initialized = 100U;       /* .data   */
uint32_t uninitialized;            /* .bss    */
static uint8_t buffer[128];         /* .bss    */
```

## Lệnh phân tích

```bash
make
arm-none-eabi-objdump -h build/embedded_foundation.elf
arm-none-eabi-nm -n build/embedded_foundation.elf
arm-none-eabi-nm -S --size-sort build/embedded_foundation.elf
arm-none-eabi-size build/embedded_foundation.elf
```

## Flash và SRAM

```text
Flash used ≈ text + data
SRAM used  ≈ data + bss
```

Stack runtime không được phản ánh đầy đủ trong dòng kết quả của `size`.

## `const` và `static`

`const` thường cho phép đặt dữ liệu vào `.rodata`, giảm SRAM. `static` thay đổi linkage/lifetime; giá trị khởi tạo mới quyết định biến thuộc `.data` hay `.bss`.
