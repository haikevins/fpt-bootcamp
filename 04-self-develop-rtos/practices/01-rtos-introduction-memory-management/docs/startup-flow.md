# Startup Flow

`common/src/startup.c` cung cấp vector table và C runtime tối thiểu.

```text
Reset -> MSP -> Reset_Handler -> copy .data -> clear .bss -> main
```

Không có C++ constructors, standard library startup hoặc semihosting.
