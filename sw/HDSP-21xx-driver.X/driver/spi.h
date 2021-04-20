/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _SPI_H_
#define _SPI_H_


#include "../types.h"


#define SPI_WRITE_OK ((int)0)
#define SPI_WRITE_INVBUFFER ((int)-1)
#define SPI_WRITE_BUSY ((int)1)


void SPI_init();

int SPI_write(const uint8_t* data, size_t count);
void SPI_blocking_write(const uint8_t* data, size_t count);

void SPI_isr();


#endif // _SPI_H_
