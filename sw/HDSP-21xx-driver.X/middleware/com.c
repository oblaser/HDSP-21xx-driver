/*!

\author         Oliver Blaser
\date           10.07.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../driver/hardware_16F690.h"
#include "../driver/uart.h"
#include "../project.h"
#include "../types.h"
#include "com.h"
#include "util.h"


#define COM_nFRAMEBYTES (3)

#define COM_CMD_ERRDEF  ((uint8_t)0xE0)
#define COM_CMD_ERRTASK ((uint8_t)0xE1)

//#define COM_ERRDEF_UNSPEC   ((uint8_t)0x00) // not used yet
#define COM_ERRDEF_BUFOF    ((uint8_t)0x01)
#define COM_ERRDEF_TIMEOUT  ((uint8_t)0x02)

#define COM_ANS_STATUS_OK       ((uint8_t)0x00)
#define COM_ANS_STATUS_ERR      ((uint8_t)0x01)
#define COM_ANS_STATUS_INVCS    ((uint8_t)0x02)
#define COM_ANS_STATUS_INVPARAM ((uint8_t)0x03)


#define RXBUFFER_SIZE ((size_t)15)
#define TXBUFFER_SIZE ((size_t)10)

#define BUFFERPOS_CMD ((size_t)0)
#define BUFFERPOS_LEN ((size_t)1)
#define BUFFERPOS_DATA ((size_t)2)

#if PRJ_DEBUG
#define T_TIMEOUT (100) // * 10ms
#else
#define T_TIMEOUT (10) // * 10ms
#endif

#define COM_DEBUGUART (PRJ_DEBUG_UART && (1))


typedef enum
{
    S_init = 0,
    S_idle,
    S_idleProc,
    S_proc,
    S_waitForApp,
    S_send,
    S_sending,
    _S_last
} state_t;


uint8_t calcCheckSum(const uint8_t* data, size_t count);
int isValidCmd(const uint8_t* data);
int isValidParam(const uint8_t* data);


#if COM_DEBUGUART
char dbgStr[36];
#endif


static uint8_t rxBuffer[RXBUFFER_SIZE];
static uint8_t txBuffer[TXBUFFER_SIZE];
static uint8_t rxIndex;
static int tmr_timeout;
static const uint8_t* const pRxData = &rxBuffer[BUFFERPOS_DATA];

static state_t state = S_init;



void COM_task(TASK_status_t* ts)
{
#if COM_DEBUGUART && 0
    const char* stateStr[] = 
    {
        "init\n",
        "idle\n",
        "idleProc\n",
        "proc\n",
        "waitForApp\n",
        "send\n",
        "sending\n",
    };
    static state_t stateOld = _S_last;
    if(stateOld != state)
    {
        UART_print_wait("COM state: ");
        UART_print_wait(stateStr[state]);
        stateOld = state;
    }
#endif
    switch(state)
    {
        case S_init:
            state = S_idle;
            break;
            
        case S_idle:
            rxIndex = 0;
            tmr_timeout = -1;
            rxBuffer[BUFFERPOS_LEN] = 0xFF - COM_nFRAMEBYTES;
            state = S_idleProc;
            break;
            
        case S_idleProc:
            {
                int rxb = UART_read();

                if(rxb == UART_READ_BUFFEROVERFLOW)
                {
                    UART_reset();
                    
                    txBuffer[0] = COM_CMD_ERRDEF;
                    txBuffer[1] = 0x01;
                    txBuffer[2] = COM_ERRDEF_BUFOF;

                    state = S_send;
                }
                else if(rxb != UART_READ_NODATA)
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
                    
#if PRJ_DEBUG
                    txBuffer[1] = 0x02;
                    txBuffer[3] = calcCheckSum(rxBuffer, rxIndex - 1);
#endif
                    
                    state = S_send;
                }
                else if(!isValidCmd(rxBuffer))
                {
                    txBuffer[0] = rxBuffer[0];
                    txBuffer[1] = 0x01;
                    txBuffer[2] = COM_ANS_STATUS_ERR;
                    
                    state = S_send;
                }
                else if(!isValidParam(rxBuffer))
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
                        // version is the only info which can be getted atm
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
                
#if COM_DEBUGUART
                size_t len;
                
                dbgStr[0] = 'R';
                dbgStr[1] = '>';
                len = UTIL_bufToHexStr(rxBuffer, ((size_t)rxBuffer[1] + COM_nFRAMEBYTES), dbgStr + 2, (sizeof(dbgStr)/sizeof(dbgStr[0])) - 2) + 2;
                dbgStr[len] = 0;
                UART_print_wait(dbgStr);
                
                dbgStr[0] = ' ';
                dbgStr[1] = '(';
                len = UTIL_byteToHexStr(cs, dbgStr + 2, (sizeof(dbgStr)/sizeof(dbgStr[0])) - 2) + 2;
                dbgStr[len] = ')';
                dbgStr[len + 1] = 0x0A;
                dbgStr[len + 2] = 0;
                UART_print_wait(dbgStr);
#endif
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
                
#if COM_DEBUGUART
                dbgStr[0] = 't';
                dbgStr[1] = '>';
                size_t len = UTIL_bufToHexStr(txBuffer, ((size_t)txBuffer[1] + COM_nFRAMEBYTES), dbgStr + 2, (sizeof(dbgStr)/sizeof(dbgStr[0])) - 2) + 2;
                dbgStr[len] = 0x0A;
                dbgStr[len + 1] = 0;
                UART_print_wait(dbgStr);
#endif
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

const uint8_t* COM_getData(size_t* len)
{
    if(len) *len = rxBuffer[BUFFERPOS_LEN];
    return pRxData;
}

uint8_t COM_APP_cmd()
{
    if(state == S_waitForApp) return rxBuffer[0];
    
    return COM_CMD_NONE;
}

void COM_APP_getDispPtr(const uint8_t** buffer, size_t* count)
{
    *buffer = pRxData;
    *count = rxBuffer[BUFFERPOS_LEN];
}

uint8_t COM_APP_getIoData()
{
    return *pRxData;
}

void COM_APP_sendOK()
{
    if(state == S_waitForApp)
    {
        txBuffer[0] = rxBuffer[0];
        txBuffer[1] = 0x01;
        txBuffer[2] = COM_ANS_STATUS_OK;

        state = S_send;
    }
}

void COM_APP_sendGetIO(uint8_t ioState)
{
    if(state == S_waitForApp)
    {
        txBuffer[0] = rxBuffer[0];
        txBuffer[1] = 0x02;
        txBuffer[2] = COM_ANS_STATUS_OK;
        txBuffer[3] = ioState;

        state = S_send;
    }
}

void COM_sendTaskError(const TASK_status_t* ts)
{    
    txBuffer[0] = COM_CMD_ERRTASK;
    txBuffer[1] = 0x01;
    txBuffer[2] = *ts;
    txBuffer[3] = calcCheckSum(txBuffer, 3);
    
    UART_write_wait(txBuffer, 4);
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
    if(data[0] == COM_CMD_NONE) return 0;
    
    if(((data[BUFFERPOS_CMD] == COM_CMD_GETINFO) && (data[BUFFERPOS_LEN] == 1)) ||
        ((data[BUFFERPOS_CMD] == COM_CMD_SETDISP) && (data[BUFFERPOS_LEN] > 0) && (data[BUFFERPOS_LEN] <= 8)) ||
        ((data[BUFFERPOS_CMD] == COM_CMD_SETDISPAT) && (data[BUFFERPOS_LEN] > 0) && (data[BUFFERPOS_LEN] <= 9)) ||
        ((data[BUFFERPOS_CMD] == COM_CMD_SETIO) && (data[BUFFERPOS_LEN] == 1)) ||
        ((data[BUFFERPOS_CMD] == COM_CMD_GETIO) && (data[BUFFERPOS_LEN] == 0)))
    {
        return 1;
    }
    
    return 0;
}

int isValidParam(const uint8_t* data)
{
    if(!data) return 0;
    
    if(((data[BUFFERPOS_CMD] == COM_CMD_GETINFO) && (data[BUFFERPOS_DATA] > 0x00)) ||
        ((data[BUFFERPOS_CMD] == COM_CMD_SETIO) && ((data[BUFFERPOS_DATA] & 0xF0) != 0)) ||
        ((data[BUFFERPOS_CMD] == COM_CMD_SETDISPAT) && (data[BUFFERPOS_DATA] > 7))) // < 0 check not neccessary because data is unsigned
    {
        return 0;
    }
    
    return 1;
}
