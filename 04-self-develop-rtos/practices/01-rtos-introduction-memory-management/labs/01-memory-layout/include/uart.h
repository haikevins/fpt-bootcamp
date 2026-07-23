#ifndef PLATFORM_UART_H
#define PLATFORM_UART_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void uart_init(uint32_t baud_rate, uint32_t peripheral_clock_hz);
void uart_write_char(char value);
void uart_write_string(const char *text);
void uart_write_line(const char *text);
void uart_write_u32(uint32_t value);
void uart_write_size(size_t value);
void uart_write_hex32(uint32_t value);
void uart_write_pointer(const void *pointer);
bool uart_try_read_char(char *value);

#endif
