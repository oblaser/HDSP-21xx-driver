/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include <stdint.h>

#include "hardware_16F690.h"
#include "uart.h"

void UART_init()
{
}

int UART_print(const char* str)
{
    return 0;
}

int UART_printf(const char* format, ...)
{
    return 0;
}

int UART_write(const uint8_t* data, size_t count)
{
    return 0;
}

int UART_isReceiving()
{
    return 0;
}

int UART_isTransmitting()
{
    return 0;
}

void UART_isrRx()
{
}

void UART_isrTx()
{
}
