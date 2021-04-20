/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include <assert.h>

#include "../driver/hardware_16F690.h"
#include "../driver/uart.h"
#include "../types.h"
#include "com.h"


#define COM_nFRAMEBYTES (3)

#define COM_CMD_GETINFO ((uint8_t)0x01)
#define COM_CMD_SETDISP ((uint8_t)0x0A)
#define COM_CMD_SETIO   ((uint8_t)0x10)
#define COM_CMD_GETIO   ((uint8_t)0x11)
#define COM_CMD_ERRDEF  ((uint8_t)0xE0)
#define COM_CMD_ERRTASK ((uint8_t)0xE1)

#define COM_ERRDEF_UNSPEC   ((uint8_t)0x00)
#define COM_ERRDEF_BUFOF    ((uint8_t)0x01)
#define COM_ERRDEF_TIMEOUT  ((uint8_t)0x02)

#define COM_ANS_STATUS_OK       ((uint8_t)0x00)
#define COM_ANS_STATUS_ERR      ((uint8_t)0x01)
#define COM_ANS_STATUS_INVPARAM ((uint8_t)0x02)
#define COM_ANS_STATUS_INVCS    ((uint8_t)0x03)


#define RXBUFFER_SIZE ((size_t)15)
#define TXBUFFER_SIZE ((size_t)10)
#define BUFFERPOS_LEN ((size_t)1)

#define T_TIMEOUT (5) // * 10ms


typedef enum
{
    S_init = 0,
    S_idle,
    S_proc,
    S_waitForApp,
    S_send,
    S_sending
} state_t;


uint8_t calcCheckSum(const uint8_t* data, size_t count);
int isValidCmd(const uint8_t* data);
int isValidParam(const uint8_t* data);


static uint8_t rxBuffer[RXBUFFER_SIZE];
static uint8_t txBuffer[TXBUFFER_SIZE];
static uint8_t rxIndex;
static int tmr_timeout;

static state_t state = S_init;



void COM_task(TASK_status_t* ts)
{
    switch(state)
    {
        case S_init:
            UART_reset();
            rxBuffer[BUFFERPOS_LEN] = 0xFF - COM_nFRAMEBYTES;
            rxIndex = 0;
            tmr_timeout = -1;
            state = S_idle;
            break;
            
        case S_idle:
            {
                int rxb = UART_read();

                if(rxb == UART_READ_BUFFEROVERFLOW)
                {
                    tmr_timeout = -1;
                }
                else if(rxb == UART_READ_NODATA)
                {
                    tmr_timeout = -1;
                }
                else
                {
                    tmr_timeout = T_TIMEOUT;

                    rxBuffer[rxIndex] = (uint8_t)rxb;
                    ++rxIndex;

                    if(rxIndex == (rxBuffer[BUFFERPOS_LEN] + COM_nFRAMEBYTES)) state = S_proc;
                }

                if(tmr_timeout == 0)
                {
                    txBuffer[0] = COM_CMD_ERRDEF;
                    txBuffer[1] = 0x01;
                    txBuffer[2] = COM_ERRDEF_TIMEOUT;

                    state = S_send;
                }
            }
            break;
            
        case S_proc:
            {
                uint8_t cs = calcCheckSum(rxBuffer, rxIndex);
                
                if(cs != 0)
                {
                    txBuffer[0] = rxBuffer[0];
                    txBuffer[1] = 0x01;
                    txBuffer[2] = COM_ANS_STATUS_INVCS;
                    
                    state = S_send;
                }
                else if(!isValidCmd(txBuffer))
                {
                    txBuffer[0] = rxBuffer[0];
                    txBuffer[1] = 0x01;
                    txBuffer[2] = COM_ANS_STATUS_ERR;
                    
                    state = S_send;
                }
                else if(!isValidParam(txBuffer))
                {
                    txBuffer[0] = rxBuffer[0];
                    txBuffer[1] = 0x01;
                    txBuffer[2] = COM_ANS_STATUS_INVPARAM;
                    
                    state = S_send;
                }
                else
                {
                    if(rxBuffer[0] == COM_CMD_GETINFO)
                    {
                        // version is the only info which can be getted
                        txBuffer[0] = rxBuffer[0];
                        txBuffer[1] = 0x04;
                        txBuffer[2] = COM_ANS_STATUS_OK;
                        txBuffer[3] = 0x00;
                        txBuffer[4] = PRJ_VERSION_MAJ;
                        txBuffer[5] = PRJ_VERSION_MIN;

                        state = S_send;
                    }
                    else
                    {
                        state = S_waitForApp;
                    }
                }
                
                rxBuffer[BUFFERPOS_LEN] = 0xFF - COM_nFRAMEBYTES;
            }
            break;
            
        case S_waitForApp:
            // nop
            break;
            
        case S_send:
            {
                size_t csPos = ((size_t)txBuffer[1] + COM_nFRAMEBYTES - 1);
                txBuffer[csPos] = calcCheckSum(txBuffer, csPos);
                state = S_sending;
            }
            break;
            
        case S_sending:
            {
                int uwr = UART_write(txBuffer, ((size_t)txBuffer[1] + COM_nFRAMEBYTES));
                
                if(uwr == UART_WRITE_OK) state = S_idle;
                else if(uwr != UART_WRITE_BUSY) *ts |= (TASK_TCOM | TASK_ERR);
            }
            break;
            
        default:
            *ts |= (TASK_TCOM | TASK_STATE);
            break;
    }
}

void COM_timeHandler()
{
    if(tmr_timeout > 0) --tmr_timeout;
}

void COM_sendTaskError(const TASK_status_t* ts)
{    
    txBuffer[0] = COM_CMD_ERRTASK;
    txBuffer[1] = 0x01;
    txBuffer[2] = *ts;
    txBuffer[3] = calcCheckSum(txBuffer, 3);
    
    UART_blocking_write(txBuffer, 4);
}



uint8_t calcCheckSum(const uint8_t* data, size_t count)
{
    uint8_t cs = *data;
    for(size_t i = 1; i < count; ++i) cs ^= *(data + i);
    return cs;
}

int isValidCmd(const uint8_t* data)
{
    if(!data) return 0;
    
    if(((data[0] == COM_CMD_GETINFO) && (data[1] == 1)) ||
        ((data[0] == COM_CMD_SETDISP) && (data[1] == 8)) ||
        ((data[0] == COM_CMD_SETIO) && (data[1] == 1)) ||
        ((data[0] == COM_CMD_GETIO) && (data[1] == 0)))
    {
        return 1;
    }
    
    return 0;
}

int isValidParam(const uint8_t* data)
{
    if(!data) return 0;
    
    if(((data[0] == COM_CMD_GETINFO) && (data[2] > 0x00)) ||
        ((data[0] == COM_CMD_SETIO) && ((data[2] & 0xF0) != 0)))
    {
        return 0;
    }
    
    return 1;
}
