/*!

\author         Oliver Blaser
\date           21.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

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

/**
 * @brief UART print
 * @param str Pointer to a null terminated string
 * @return UART_WRITE_OK or UART_WRITE_BUSY or UART_WRITE_INVBUFFER
 */
int UART_print(const char* str)
{
    if(!str) return UART_WRITE_INVBUFFER;
    
    size_t n = 0;
    while((*(str + n) != 0) && (n <= TX_BUF_SIZE)) ++n;
    
    return UART_write((const uint8_t*)str, n);
}

/**
 * @brief UART write, blocking
 * @param data Pointer to the data buffer
 * @param count Number of bytes to be transmitted
 * @return UART_WRITE_OK or UART_WRITE_BUSY or UART_WRITE_INVBUFFER
 */
int UART_write(const uint8_t* data, size_t count)
{
    if(TXIE) return UART_WRITE_BUSY;
    if((count > ((size_t)TX_BUF_SIZE)) || (!data)) return UART_WRITE_INVBUFFER;
    
    txSize = count;
    txIndex = 0;
    
    for(size_t i = 0; i < count; ++i) txBuf[i] = data[i];
    
    TXIE = 1;
    
    return UART_WRITE_OK;
}

/**
 * @brief UART print, blocking
 * @param str Pointer to a null terminated string
 * @return UART_WRITE_OK or UART_WRITE_INVBUFFER
 * 
 * Waits until a potential ongoing transmission is finished and then starts the
 * transmission of <i>str</i>. Does not return until this transmission is finnished.
 */
int UART_print_blocking(const char* str)
{
    int r = UART_print_wait(str);
    while(UART_isTransmitting()) asm("NOP");
    return r;
}

/**
 * @brief UART write, blocking
 * @param data Pointer to the data buffer
 * @param count Number of bytes to be transmitted
 * @return UART_WRITE_OK or UART_WRITE_INVBUFFER
 * 
 * Waits until a potential ongoing transmission is finished and then starts the
 * transmission of <i>data</i>. Does not return until this transmission is finnished.
 */
int UART_write_blocking(const uint8_t* data, size_t count)
{
    int r = UART_write_wait(data, count);
    while(UART_isTransmitting()) asm("NOP");
    return r;
}

/**
 * @brief UART print, waiting
 * @param str Pointer to a null terminated string
 * @return UART_WRITE_OK or UART_WRITE_INVBUFFER
 * 
 * Waits until a potential ongoing transmission is finished and then starts the
 * transmission of <i>str</i>.
 */
int UART_print_wait(const char* str)
{
    int r = UART_WRITE_BUSY;
    while(r == UART_WRITE_BUSY) r = UART_print(str);
    return r;
}

/**
 * @brief UART write, waiting
 * @param data Pointer to the data buffer
 * @param count Number of bytes to be transmitted
 * @return UART_WRITE_OK or UART_WRITE_INVBUFFER
 * 
 * Waits until a potential ongoing transmission is finished and then starts the
 * transmission of <i>data</i>.
 */
int UART_write_wait(const uint8_t* data, size_t count)
{
    int r = UART_WRITE_BUSY;
    while(r == UART_WRITE_BUSY) r = UART_write(data, count);
    return r;
}

/**
 * @brief Get data from the rx buffer
 * @return UART_READ_NODATA or UART_READ_BUFFEROVERFLOW or the next data byte
 */
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
#if((RX_BUF_SIZE < 2) || (RX_BUF_SIZE > 0x7FFF) || (TX_BUF_SIZE < 1) || (TX_BUF_SIZE > 0x7FFF))
#error invalid config
#endif
