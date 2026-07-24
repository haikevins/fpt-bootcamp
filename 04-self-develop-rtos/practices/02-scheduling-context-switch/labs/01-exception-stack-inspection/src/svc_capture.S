.syntax unified
.cpu cortex-m3
.thumb

.extern lab_svc_capture

.global SVC_Handler
.type SVC_Handler, %function
.thumb_func
SVC_Handler:
    mrs     r0, msp
    mrs     r1, psp
    mov     r2, lr
    push    {r3, lr}
    bl      lab_svc_capture
    pop     {r3, lr}
    bx      lr
.size SVC_Handler, .-SVC_Handler
