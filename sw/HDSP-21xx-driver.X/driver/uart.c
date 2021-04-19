/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include <stdint.h>

#include "hardware_16F690.h"
#include "uart.h"



#define RX_BUF_SIZE (5)
#define TX_BUF_SIZE (30)


void incRxIndexRd();
void incRxIndexWr();


static uint8_t rxBuf[RX_BUF_SIZE];
static uint8_t txBuf[TX_BUF_SIZE];

static uint8_t rxBufferOverflow;
static int rxIndexRd;
static int rxIndexWr;
static size_t txIndex;
static size_t txSize;



void UART_init()
{
    // TX & RX pins must be set as Input first
    // set Baudrate 19200 with an Error of 0.16%
    SPBRG = 25;
    SPBRGH = 0;
    BRGH = 0;
    BRG16 = 1;

    // setup EUSART as Asynchronous TRX
    SYNC = 0;
    SPEN = 1;
    RCIE = 1;
    CREN = 1;
    TXEN = 1;
    
    UART_reset();
}

void UART_reset()
{
    TXIE = 0;
    rxBufferOverflow = 0;
    rxIndexRd = 0;
    rxIndexWr = 0;
    txIndex = 0;
    txSize = 0;
}

int UART_isTransmitting()
{
    if(TXIE) return 1;
    return 0;
}

int UART_print(const char* str)
{
    size_t n = 0;
    
    while(*(str + n) != 0) ++n;
    
    return UART_write((const uint8_t*)str, n);
}

int UART_write(const uint8_t* data, size_t count)
{
    if(count > ((size_t)TX_BUF_SIZE)) return 2;
    if(TXIE) return 1;
    
    txSize = count;
    txIndex = 0;
    
    for(size_t i = 0; i < count; ++i) txBuf[i] = data[i];
    
    TXIE = 1;
    
    return 0;
}

void UART_blocking_print(const char* str)
{
    int r = 1;
    while(r == 1) r = UART_print(str);
}

void UART_blocking_write(const uint8_t* data, size_t count)
{
    int r = 1;
    while(r == 1) r = UART_write(data, count);
}

int UART_read()
{
    if(rxBufferOverflow) return UART_READ_BUFFEROVERFLOW;
    if(rxIndexRd == rxIndexWr) return UART_READ_NODATA;
    
    int r = rxBuf[rxIndexRd];
    
    incRxIndexRd();
        
    return r;
}

void UART_rx_isr()
{
    rxBuf[rxIndexWr] = RCREG;
    incRxIndexWr();
    RCIF = 0;
}

void UART_tx_isr()
{
    TXREG = txBuf[txIndex++];
    
    if(txIndex >= txSize) TXIE = 0; // TXIF is automatically setted by EUSART module
                                    // and allways 1 if the TX reg is emty
}



void incRxIndexRd()
{
    ++rxIndexRd;
    if(rxIndexRd >= RX_BUF_SIZE) rxIndexRd = 0;
}

void incRxIndexWr()
{
    ++rxIndexWr;
    if(rxIndexWr >= RX_BUF_SIZE) rxIndexWr = 0;
    
    if(rxIndexWr == rxIndexRd) rxBufferOverflow = 1;
}


// invalid config check
#if((RX_BUF_SIZE < 2) || (RX_BUF_SIZE > 0x7FFF) || (TX_BUF_SIZE > 0x7FFF))
#error invalid config
#endif
