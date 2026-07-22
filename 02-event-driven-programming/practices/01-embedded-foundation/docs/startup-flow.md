# Startup Flow

## Luồng tổng quát

```text
Power-on / Reset
       |
       v
CPU đọc word tại 0x08000000
       |
       v
Nạp Main Stack Pointer
       |
       v
CPU đọc word tại 0x08000004
       |
       v
Nhảy tới Reset_Handler
       |
       +--> Copy .data
       |
       +--> Clear .bss
       |
       v
main()
```

## Vector Table

Hai entry đầu:

```text
vector[0] = &_estack
vector[1] = Reset_Handler
```

Linker giữ section bằng:

```ld
KEEP(*(.isr_vector))
```

## Copy `.data`

Biến đã khởi tạo cần giá trị nguồn trong Flash và vị trí chạy trong SRAM.

```text
Flash [_sidata ...]
        |
        | copy
        v
SRAM  [_sdata ... _edata)
```

## Clear `.bss`

Vùng `[_sbss, _ebss)` được ghi bằng 0 trước khi gọi `main()`.

## Debug checklist

- SP có nằm trong SRAM?
- Vector Table có bắt đầu tại Flash origin?
- `.data` có đúng giá trị trước `main()`?
- `.bss` có bằng 0?
- Có rơi vào `Default_Handler` không?
