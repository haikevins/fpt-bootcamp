# Lab 01 - Endianness

## Mục tiêu

- Hiểu cách giá trị 32-bit được chia thành bốn byte.
- Hiểu quan hệ giữa biến, địa chỉ và con trỏ byte.
- Phân biệt little-endian và big-endian.

## Build và chạy

Từ thư mục lab:

```bash
cd labs/01-endianness
make
make run
```

Artifact được tạo tại:

```text
build/lab01_endianness
```

## Kết quả dự kiến trên little-endian

```text
byte[0] = 0x78
byte[1] = 0x56
byte[2] = 0x34
byte[3] = 0x12
```

`0x78` là byte có trọng số thấp nhất nên được lưu tại địa chỉ thấp nhất.

## Câu hỏi

1. Endianness có làm thay đổi giá trị logic của `value` không?
2. Tại sao x86 và Cortex-M thường cho cùng kết quả trong lab này?
3. Khi truyền dữ liệu qua network, hai bên cần thống nhất byte order như thế nào?
