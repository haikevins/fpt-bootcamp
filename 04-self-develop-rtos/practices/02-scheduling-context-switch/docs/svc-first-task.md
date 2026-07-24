# SVC first-task startup

`main()` chạy bằng MSP. Kernel chọn TCB đầu tiên rồi phát lệnh SVC. SVC handler
lấy `saved_sp`, restore R4-R11, ghi PSP, chọn PSP qua CONTROL và exception-return
với `0xFFFFFFFD`. `main()` không được tiếp tục sau khi kernel đã start.
