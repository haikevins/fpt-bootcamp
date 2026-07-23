# Validation

Các thay đổi đã được kiểm tra như sau:

- Makefile gốc không còn các rule `lab01` ... `lab08`, `host-labs` hoặc `all-labs`.
- Makefile gốc chỉ quản lý firmware tổng kết và các lệnh phân tích/flash của firmware đó.
- Cả 8 lab đều có Makefile riêng.
- Lab 01, 02 và 03 đã build và chạy thành công trên host.
- Makefile của Lab 04 đến Lab 08 đã được parse và dry-run thành công.
- Root Makefile đã được dry-run thành công.
- README gốc và README của từng lab đã được cập nhật để dùng Makefile cục bộ.
- Build artifact cũ trong các lab đã được loại khỏi bản ZIP.

Lưu ý: firmware ARM chưa được cross-build thực tế trong môi trường tạo file vì
`arm-none-eabi-gcc` không được cài đặt. Dependency graph và command của các Makefile
ARM đã được kiểm tra bằng `make -n`.
