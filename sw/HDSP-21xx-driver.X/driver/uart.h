/*!

\author         Oliver Blaser
\date           21.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _UART_H_
#define _UART_H_

#include "../types.h"


//! Returned if the transmission has been started
#define UART_WRITE_OK ((int)0)

//! Returned if the data pointer is null
#define UART_WRITE_INVBUFFER ((int)-1)

//! Returned if a transmission is ongoing
#define UART_WRITE_BUSY ((int)1)

//! Returned if no new data is available
#define UART_READ_NODATA ((int)-1)

//! Returned if the rx buffer overflowed
#define UART_READ_BUFFEROVERFLOW ((int)-2)


void UART_init();

void UART_reset();
int UART_isTransmitting();

int UART_print(const char* str);
int UART_write(const uint8_t* data, size_t count);

int UART_print_blocking(const char* str);
int UART_write_blocking(const uint8_t* data, size_t count);
int UART_print_wait(const char* str);
int UART_write_wait(const uint8_t* data, size_t count);

int UART_read();

void UART_rx_isr();
void UART_tx_isr();

#endif // _UART_H_
