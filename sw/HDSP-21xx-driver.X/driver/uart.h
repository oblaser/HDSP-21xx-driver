/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include <string.h>

#include "../project.h"

void UART_init();

int UART_print(const char* str);
int UART_printf(const char* format, ...);
int UART_write(const uint8_t* data, size_t count);

int UART_isReceiving();
int UART_isTransmitting();

void UART_isrRx();
void UART_isrTx();

#endif // _UART_H_
