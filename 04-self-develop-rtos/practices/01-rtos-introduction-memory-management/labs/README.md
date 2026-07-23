# Labs — independent Makefiles

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile gốc.
- Không dùng rule tổng hợp như `make lab01`, `make all-labs` hoặc `make run-host-labs`.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output được tạo trong `build/` bên trong chính lab.
- `make clean` chỉ xóa output của lab hiện tại.

## Target labs

```bash
cd labs/01-memory-layout
make

cd ../02-startup-sections
make

cd ../10-target-demo
make
```

Khi máy không có GNU Arm Embedded Toolchain:

```bash
make TOOLCHAIN=clang
```

## Host labs

```bash
cd labs/05-first-fit-basic
make test
make run
```

Các lab host dùng AddressSanitizer và UndefinedBehaviorSanitizer.
