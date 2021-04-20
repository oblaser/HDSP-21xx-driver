/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../types.h"
#include "hardware_16F690.h"
#include "spi.h"


#define TX_BUF_SIZE (20)

static uint8_t txBuf[TX_BUF_SIZE];
static size_t txIndex;
static size_t txSize;


void SPI_init()
{
    SSPCONbits.SSPM0 = 0;
    SSPCONbits.SSPM1 = 1;
    SSPCONbits.SSPM2 = 0;
    SSPCONbits.SSPM3 = 0;
    
    SSPCONbits.SSPEN = 1;
    
    SSPCONbits.CKP = 0;
    SSPSTATbits.CKE = 1;
    
    SSPSTATbits.SMP = 0;
    
    SSPIE = 0;
}

int SPI_write(const uint8_t* data, size_t count)
{
    if(SSPIE) return SPI_WRITE_BUSY;
    if((count > ((size_t)TX_BUF_SIZE)) || (count < 1) || (!data)) return SPI_WRITE_INVBUFFER;
    
    txSize = count;
    txIndex = 0;
    
    for(size_t i = 0; i < count; ++i) txBuf[i] = data[i];
    
    SSPBUF = txBuf[txIndex++];
    
    SSPIE = 1;
    
    return SPI_WRITE_OK;
}

// with SSPM = 0b0001 (Fosc/16) and two bytes the function takes 69us (compiler -O2)
void SPI_blocking_write(const uint8_t* data, size_t count)
{
    if(data)
    {
        size_t i = 0;   
        while(i < count) // faster than for
        {
            SSPBUF = data[i++];
            while(SSPSTATbits.BF == 0) asm("NOP");
        }
    }
}

void SPI_isr()
{
    if(txIndex < txSize) SSPBUF = txBuf[txIndex++];
    else SSPIE = 0;
    
    SSPIF = 0;
}
