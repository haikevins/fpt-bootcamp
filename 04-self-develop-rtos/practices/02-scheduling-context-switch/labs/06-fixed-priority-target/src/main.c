#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "systick.h"
#include "task.h"
#include "uart.h"

#include <stdint.h>

static hr_task_t g_high, g_mid, g_low, g_idle;
static uint32_t s_high[128] __attribute__((aligned(8)));
static uint32_t s_mid[128] __attribute__((aligned(8)));
static uint32_t s_low[128] __attribute__((aligned(8)));
static uint32_t s_idle[96] __attribute__((aligned(8)));
volatile uint32_t high_count, mid_count, low_count, idle_count;

static void high(void *arg)
{
    (void)arg;
    for (;;)
    {
        ++high_count;
        gpio_led_toggle();
        hr_task_delay(100U);
    }
}

static void mid(void *arg)
{
    (void)arg;
    for (;;)
    {
        ++mid_count;
        hr_task_delay(10U);
    }
}

static void low(void *arg)
{
    (void)arg;
    for (;;)
    {
        ++low_count;
        hr_task_delay(1U);
    }
}

static void idle(void *arg)
{
    (void)arg;
    for (;;)
    {
        ++idle_count;
        hr_port_wait_for_interrupt();
    }
}

int main(void)
{
    clock_init_hsi_8mhz(); gpio_led_init(); uart1_init_9600_hsi8();
    uart1_write_string("\r\nLab 06 fixed priority\r\n");
    hr_scheduler_init();
    HR_ASSERT(hr_task_create_static(&g_high,"high",0U,0U,high,0,s_high,128U));
    HR_ASSERT(hr_task_create_static(&g_mid,"mid",1U,1U,mid,0,s_mid,128U));
    HR_ASSERT(hr_task_create_static(&g_low,"low",2U,2U,low,0,s_low,128U));
    HR_ASSERT(hr_task_create_static(&g_idle,"idle",3U,3U,idle,0,s_idle,96U));
    HR_ASSERT(hr_scheduler_add_task(&g_high)); HR_ASSERT(hr_scheduler_add_task(&g_mid));
    HR_ASSERT(hr_scheduler_add_task(&g_low)); HR_ASSERT(hr_scheduler_add_task(&g_idle));
    systick_init_1khz(); hr_scheduler_start();
}
