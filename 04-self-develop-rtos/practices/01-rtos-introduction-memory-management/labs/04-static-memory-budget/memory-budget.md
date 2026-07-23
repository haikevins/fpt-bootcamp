# Static Memory Budget Worksheet

| Object | Count | Bytes/object | Total |
|---|---:|---:|---:|
| Task stack | 4 | 512 | 2048 |
| TCB estimate | 4 | 96 | 384 |
| Queue storage | 2 | 256 | 512 |
| Mutex | 3 | 32 | 96 |
| Software timer | 4 | 40 | 160 |
| Event pool | 1 | 512 | 512 |
| Main stack reserve | 1 | 1024 | 1024 |
| **Total** | | | **4736** |

STM32F103C8T6 SRAM: 20480 bytes. Phần còn lại phải chứa `.data`, `.bss`, driver
buffers, kernel lists và safety margin.
