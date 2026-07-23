#ifndef PLATFORM_GPIO_H
#define PLATFORM_GPIO_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B,
    GPIO_PORT_C
} gpio_port_t;

void gpio_enable_clock(gpio_port_t port);
void gpio_config_output_push_pull(gpio_port_t port, uint32_t pin);
void gpio_config_alternate_push_pull(gpio_port_t port, uint32_t pin);
void gpio_config_input_floating(gpio_port_t port, uint32_t pin);
void gpio_write(gpio_port_t port, uint32_t pin, bool high);
void gpio_toggle(gpio_port_t port, uint32_t pin);

#endif
