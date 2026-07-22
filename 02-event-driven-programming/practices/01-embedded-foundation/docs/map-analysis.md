# Map File Analysis

## Map file

```text
build/embedded_foundation.map
```

Map file cho biết:

- Object file đóng góp vào section.
- Địa chỉ và kích thước section.
- Địa chỉ symbol.
- VMA và LMA.
- Section bị loại bởi garbage collection.

## Tạo map

```bash
make
```

Makefile dùng:

```makefile
-Wl,-Map=build/embedded_foundation.map
```

## Phần cần tìm

```text
Discarded input sections
Memory Configuration
Linker script and memory map
.isr_vector
.text
.data
.bss
```

## Symbol quan trọng

```text
_estack
_sidata
_sdata
_edata
_sbss
_ebss
_etext
```

## Lệnh hỗ trợ

```bash
arm-none-eabi-size build/embedded_foundation.elf
arm-none-eabi-nm -n build/embedded_foundation.elf
arm-none-eabi-nm -S --size-sort build/embedded_foundation.elf
arm-none-eabi-objdump -h build/embedded_foundation.elf
```

## Báo cáo đề xuất

| Chỉ số | Giá trị |
|---|---:|
| `.text` | ... bytes |
| `.data` | ... bytes |
| `.bss` | ... bytes |
| Flash used | ... bytes |
| SRAM static used | ... bytes |
| SRAM còn lại | ... bytes |

## Câu hỏi

1. Vì sao `.data` làm tăng cả Flash và SRAM?
2. Vì sao `.bss` không làm binary tăng tương ứng toàn bộ kích thước?
3. Symbol nào lớn nhất?
4. Section nào bị loại bởi `--gc-sections`?
5. Stack có được tính đầy đủ bởi `arm-none-eabi-size` không?
