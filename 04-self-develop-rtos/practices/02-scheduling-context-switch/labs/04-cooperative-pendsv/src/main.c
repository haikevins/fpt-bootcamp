#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "task.h"
#include "uart.h"

#include <stdint.h>

static hr_task_t g_task_a;
static hr_task_t g_task_b;
static hr_task_t g_idle;
static uint32_t g_stack_a[128] __attribute__((aligned(8)));
static uint32_t g_stack_b[128] __attribute__((aligned(8)));
static uint32_t g_stack_idle[96] __attribute__((aligned(8)));
volatile uint32_t g_task_a_value;
volatile uint32_t g_task_b_value;

static void task_a(void *argument)
{
    uint32_t local = 0xA0000000UL;
    (void)argument;
    for (;;)
    {
        ++local;
        g_task_a_value = local;
        hr_task_yield();
    }
}

static void task_b(void *argument)
{
    uint32_t local = 0xB0000000UL;
    (void)argument;
    for (;;)
    {
        local += 2U;
        g_task_b_value = local;
        gpio_led_toggle();
        hr_task_yield();
    }
}

static void idle(void *argument)
{
    (void)argument;
    for (;;)
    {
        hr_port_wait_for_interrupt();
    }
}

int main(void)
{
    clock_init_hsi_8mhz();
    gpio_led_init();
    uart1_init_9600_hsi8();
    uart1_write_string("\r\nLab 04 cooperative PendSV\r\n");
    hr_scheduler_init();

    HR_ASSERT(hr_task_create_static(&g_task_a, "A", 0U, 1U,
                                    task_a, (void *)0, g_stack_a, 128U));
    HR_ASSERT(hr_task_create_static(&g_task_b, "B", 1U, 1U,
                                    task_b, (void *)0, g_stack_b, 128U));
    HR_ASSERT(hr_task_create_static(&g_idle, "idle", 2U, 3U,
                                    idle, (void *)0, g_stack_idle, 96U));
    HR_ASSERT(hr_scheduler_add_task(&g_task_a));
    HR_ASSERT(hr_scheduler_add_task(&g_task_b));
    HR_ASSERT(hr_scheduler_add_task(&g_idle));
    hr_scheduler_start();
}
