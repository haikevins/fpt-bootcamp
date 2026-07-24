# Cortex-M3 exception model

Task chạy ở Thread mode bằng PSP. Exception chạy ở Handler mode bằng MSP.
Khi exception entry, CPU tự lưu R0-R3, R12, LR, PC và xPSR. PendSV lưu thêm
R4-R11. Exception return `0xFFFFFFFD` phục hồi hardware frame và quay về
Thread mode bằng PSP.
