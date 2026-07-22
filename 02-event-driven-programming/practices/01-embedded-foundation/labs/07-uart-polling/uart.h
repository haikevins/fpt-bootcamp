#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stdint.h>

bool uart1_init(uint32_t peripheral_clock_hz, uint32_t baud_rate);
void uart1_send_byte(uint8_t byte);
void uart1_send_string(const char *text);
bool uart1_byte_available(void);
uint8_t uart1_read_byte(void);

#endif
