# STM32F103C8T6 Memory Map

## Tổng quan

```text
0x08000000 +------------------------------+
           | Main Flash — 64 KiB          |
           | 0x08000000..0x0800FFFF       |
0x08010000 +------------------------------+

0x20000000 +------------------------------+
           | SRAM — 20 KiB                |
           | 0x20000000..0x20004FFF       |
0x20005000 +------------------------------+

0x40000000 +------------------------------+
           | Peripheral registers         |
0x60000000 +------------------------------+

0xE0000000 +------------------------------+
           | Cortex-M system region       |
           | NVIC, SysTick, SCB, debug    |
0xFFFFFFFF +------------------------------+
```

## Flash

```text
Origin: 0x08000000
Length: 64 KiB
End   : 0x0800FFFF
```

Chứa Vector Table, `.text`, `.rodata` và giá trị khởi tạo của `.data`.

## SRAM

```text
Origin: 0x20000000
Length: 20 KiB
End   : 0x20004FFF
Top   : 0x20005000
```

Chứa `.data`, `.bss`, biến local và stack. `_estack` được đặt tại `0x20005000`.

## Peripheral dùng trong project

| Peripheral | Base address |
|---|---:|
| GPIOA | `0x40010800` |
| GPIOC | `0x40011000` |
| USART1 | `0x40013800` |
| RCC | `0x40021000` |

## Thanh ghi dùng trong code

| Thanh ghi | Địa chỉ |
|---|---:|
| `GPIOA_CRH` | `0x40010804` |
| `GPIOC_CRH` | `0x40011004` |
| `GPIOC_ODR` | `0x4001100C` |
| `GPIOC_BSRR` | `0x40011010` |
| `USART1_SR` | `0x40013800` |
| `USART1_DR` | `0x40013804` |
| `USART1_BRR` | `0x40013808` |
| `USART1_CR1` | `0x4001380C` |
| `RCC_APB2ENR` | `0x40021018` |

Memory map phải luôn được đối chiếu với datasheet, reference manual và part number thực tế.
