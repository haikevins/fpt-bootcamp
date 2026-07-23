#include "compiler.h"
#include "gpio.h"
#include "memory_explorer.h"
#include "uart.h"
#include <stddef.h>
#include <stdint.h>

#define HSI_CLOCK_HZ (8000000UL)
#define UART_BAUD_RATE (9600UL)
#define LINE_CAPACITY (80U)

static uint8_t g_heap_storage[4096] ATTR_ALIGNED(8);

int main(void)
{
    char line[LINE_CAPACITY];
    size_t length = 0U;
    char value;

    gpio_enable_clock(GPIO_PORT_C);
    gpio_config_output_push_pull(GPIO_PORT_C, 13UL);
    gpio_write(GPIO_PORT_C, 13UL, true);

    uart_init(UART_BAUD_RATE, HSI_CLOCK_HZ);
    uart_write_line("RTOS Topic 01 - Memory Explorer");
    uart_write_line("Clock: HSI 8 MHz");
    uart_write_line("USART1: 9600 8-N-1");

    if (!memory_explorer_init(g_heap_storage, sizeof(g_heap_storage)))
    {
        uart_write_line("Heap initialization: FAIL");
    }

    memory_explorer_print_help();
    uart_write_string("> ");

    for (;;)
    {
        if (!uart_try_read_char(&value))
        {
            continue;
        }

        if ((value == '\r') || (value == '\n'))
        {
            if (length > 0U)
            {
                line[length] = '\0';
                uart_write_string("\r\n");
                memory_explorer_execute(line);
                length = 0U;
                uart_write_string("> ");
            }
        }
        else if ((value == '\b') || (value == 0x7F))
        {
            if (length > 0U)
            {
                length--;
                uart_write_string("\b \b");
            }
        }
        else if (length + 1U < LINE_CAPACITY)
        {
            line[length++] = value;
            uart_write_char(value);
        }
    }
}
