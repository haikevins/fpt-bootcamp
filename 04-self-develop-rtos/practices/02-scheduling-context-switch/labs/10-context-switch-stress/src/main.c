#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "systick.h"
#include "task.h"
#include "uart.h"

#include <stdint.h>

static hr_task_t high_t,a,b,c,idle_t;
static uint32_t sh[128] __attribute__((aligned(8)));
static uint32_t sa[128] __attribute__((aligned(8)));
static uint32_t sb[128] __attribute__((aligned(8)));
static uint32_t sc[128] __attribute__((aligned(8)));
static uint32_t si[96] __attribute__((aligned(8)));
volatile uint32_t vh,va,vb,vc,guard_failures;

static void high(void *x)
{
    (void)x;
    for (;;)
    {
        ++vh;
        if (!hr_task_stack_guard_ok(&high_t) ||
            !hr_task_stack_guard_ok(&a) ||
            !hr_task_stack_guard_ok(&b) ||
            !hr_task_stack_guard_ok(&c) ||
            !hr_task_stack_guard_ok(&idle_t))
        {
            ++guard_failures;
            hr_panic(HR_PANIC_STACK_CORRUPTION, guard_failures);
        }
        gpio_led_toggle();
        hr_task_delay(100U);
    }
}
static void wa(void *x){uint32_t l=0x11110000UL;(void)x;for(;;){++l;va=l;}}
static void wb(void *x){uint32_t l=0x22220000UL;(void)x;for(;;){l+=3U;vb=l;}}
static void wc(void *x){uint32_t l=0x33330000UL;(void)x;for(;;){l+=5U;vc=l;}}
static void idle(void *x){(void)x;for(;;){hr_port_wait_for_interrupt();}}

int main(void)
{
    clock_init_hsi_8mhz(); gpio_led_init(); uart1_init_9600_hsi8();
    uart1_write_string("\r\nLab 10 context switch stress\r\n");
    hr_scheduler_init();
    HR_ASSERT(hr_task_create_static(&high_t,"high",0U,0U,high,0,sh,128U));
    HR_ASSERT(hr_task_create_static(&a,"A",1U,1U,wa,0,sa,128U));
    HR_ASSERT(hr_task_create_static(&b,"B",2U,1U,wb,0,sb,128U));
    HR_ASSERT(hr_task_create_static(&c,"C",3U,1U,wc,0,sc,128U));
    HR_ASSERT(hr_task_create_static(&idle_t,"idle",4U,3U,idle,0,si,96U));
    HR_ASSERT(hr_scheduler_add_task(&high_t)); HR_ASSERT(hr_scheduler_add_task(&a));
    HR_ASSERT(hr_scheduler_add_task(&b)); HR_ASSERT(hr_scheduler_add_task(&c));
    HR_ASSERT(hr_scheduler_add_task(&idle_t));
    systick_init_1khz(); hr_scheduler_start();
}
