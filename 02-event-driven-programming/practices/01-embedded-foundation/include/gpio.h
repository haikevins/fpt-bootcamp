#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

void gpio_led_init(void);
void gpio_led_on(void);
void gpio_led_off(void);
void gpio_led_write(bool enabled);
void gpio_led_toggle(void);
bool gpio_led_is_on(void);

#endif
