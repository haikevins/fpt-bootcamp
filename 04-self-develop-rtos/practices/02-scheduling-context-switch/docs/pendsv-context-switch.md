# PendSV context switch

PendSV có priority thấp nhất. Handler save R4-R11 vào PSP hiện tại, lưu PSP vào
TCB, gọi scheduler chọn task tiếp theo, restore R4-R11 từ stack mới, ghi PSP và
exception-return. PendSV không gọi UART, allocator hoặc application callback.
