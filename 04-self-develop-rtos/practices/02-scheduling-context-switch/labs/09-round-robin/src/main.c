#include "clock.h"
#include "cortex_m3_port.h"
#include "panic.h"
#include "scheduler.h"
#include "systick.h"
#include "task.h"
#include "uart.h"

#include <stdint.h>

static hr_task_t ta,tb,tc,ti;
static uint32_t sa[112] __attribute__((aligned(8)));
static uint32_t sb[112] __attribute__((aligned(8)));
static uint32_t sc[112] __attribute__((aligned(8)));
static uint32_t si[96] __attribute__((aligned(8)));
volatile uint32_t ca,cb,cc;

static void wa(void *x){(void)x; for(;;){++ca;}}
static void wb(void *x){(void)x; for(;;){cb+=2U;}}
static void wc(void *x){(void)x; for(;;){cc+=3U;}}
static void idle(void *x){(void)x; for(;;){hr_port_wait_for_interrupt();}}

int main(void)
{
    clock_init_hsi_8mhz(); uart1_init_9600_hsi8();
    uart1_write_string("\r\nLab 09 round robin\r\n"); hr_scheduler_init();
    HR_ASSERT(hr_task_create_static(&ta,"A",0U,1U,wa,0,sa,112U));
    HR_ASSERT(hr_task_create_static(&tb,"B",1U,1U,wb,0,sb,112U));
    HR_ASSERT(hr_task_create_static(&tc,"C",2U,1U,wc,0,sc,112U));
    HR_ASSERT(hr_task_create_static(&ti,"idle",3U,3U,idle,0,si,96U));
    HR_ASSERT(hr_scheduler_add_task(&ta)); HR_ASSERT(hr_scheduler_add_task(&tb));
    HR_ASSERT(hr_scheduler_add_task(&tc)); HR_ASSERT(hr_scheduler_add_task(&ti));
    systick_init_1khz(); hr_scheduler_start();
}
