# Lab 08 - Map File Analysis

Project standalone có code cố ý tạo các object với kích thước dễ quan sát.

| Object | Section | Kích thước |
|---|---|---:|
| `g_lookup_table` | `.rodata` | 256 bytes |
| `g_initialized_statistics` | `.data` | 64 bytes |
| `g_rx_buffer` | `.bss` | 512 bytes |
| `g_framebuffer` | `.bss` | 1024 bytes |

## Build

```bash
cd labs/08-map-analysis
make
make report
./analyze.sh
```

Hoặc:

```bash
make lab08
```

Phân tích:

```text
build/lab08_map_analysis.map
build/lab08_map_analysis.lst
build/size-report.txt
build/section-report.txt
build/symbol-report.txt
```
