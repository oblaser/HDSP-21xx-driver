/*!

\author         Oliver Blaser
\date           21.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../driver/gpio.h"
#include "../middleware/com.h"
#include "../middleware/hdsp_21xx.h"
#include "../middleware/shiftReg.h"
#include "../types.h"
#include "appMain.h"


#define IOSTATE_BUT         ((uint8_t)0x10)
#define IOSTATE_BUT_EDGER   ((uint8_t)0x20)
#define IOSTATE_BUT_EDGEF   ((uint8_t)0x40)


typedef enum
{
    S_init = 0,
    S_idle,
    S_setDisp,
    S_setIO,
    S_getIO,
    _S_last
} state_t;


static uint8_t ioState;



void APP_task(TASK_status_t* ts)
{
    static state_t state = S_init;
    
    GPIO_input_t inp;
    GPIO_inpDetect(&inp);
    if(inp.rising & GPIO_INP_BUTTON) ioState |= IOSTATE_BUT_EDGER;
    if(inp.falling & GPIO_INP_BUTTON) ioState |= IOSTATE_BUT_EDGEF;
    
    switch(state)
    {
        case S_init:
            HDSP_init();
            state = S_idle;
            break;
            
        case S_idle:
            {
                uint8_t cmd = COM_APP_cmd();
                
                if(cmd == COM_CMD_SETDISP) state = S_setDisp;
                else if(cmd == COM_CMD_SETIO) state = S_setIO;
                else if(cmd == COM_CMD_GETIO) state = S_getIO;
                else if(cmd != COM_CMD_NONE) *ts |= (TASK_TAPP | TASK_INVCMD);
            }
            break;
            
        case S_setDisp:
            {
                HDSP_printAt((const char*)COM_APP_getDispPtr(), 0, 8);
                COM_APP_sendOK();
                state = S_idle;
            }
            break;
            
        case S_setIO:
            {
                SHR_LED_set(COM_APP_getIoData());
                COM_APP_sendOK();
                state = S_idle;
            }
            break;
            
        case S_getIO:
            {
                if(inp.state & GPIO_INP_BUTTON) ioState |= IOSTATE_BUT;
                ioState |= (SHR_LED_get() & 0x0F);
                
                COM_APP_sendGetIO(ioState);
                
                ioState = 0;
                state = S_idle;
            }
            break;
            
        default:
            *ts |= (TASK_TAPP | TASK_STATE);
            break;
    }
}

void APP_timeHandler()
{
}
