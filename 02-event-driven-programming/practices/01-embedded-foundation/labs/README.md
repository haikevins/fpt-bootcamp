# Labs

Mỗi thư mục trong `labs/` là một project thực hành độc lập và có `Makefile` riêng.
Makefile ở root chỉ quản lý firmware tổng kết, không build hoặc clean các lab.

## Cách sử dụng

```bash
cd labs/01-endianness
make
make run
make clean
```

Với các lab chạy trên STM32:

```bash
cd labs/06-gpio-register
make
make flash-stlink
make clean
```

## Danh sách

| Lab | Môi trường | Lệnh chính |
|---:|---|---|
| 01 Endianness | Host | `make`, `make run` |
| 02 Memory access | Host | `make`, `make run` |
| 03 Memory sections | Host | `make`, `make run`, `make report` |
| 04 Startup code | STM32F103 | `make`, `make report`, `make flash-stlink` |
| 05 Linker script | STM32F103 | `make`, `make report`, `make flash-stlink` |
| 06 GPIO register | STM32F103 | `make`, `make report`, `make flash-stlink` |
| 07 UART polling | STM32F103 | `make`, `make report`, `make flash-stlink` |
| 08 Map analysis | STM32F103 | `make`, `make report`, `./analyze.sh` |
