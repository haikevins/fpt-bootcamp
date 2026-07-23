#ifndef PLATFORM_SYSTICK_H
#define PLATFORM_SYSTICK_H

#include <stdbool.h>
#include <stdint.h>

bool systick_init(uint32_t core_clock_hz, uint32_t tick_hz);
uint32_t systick_now(void);
void systick_delay_ms(uint32_t delay_ms);

#endif
