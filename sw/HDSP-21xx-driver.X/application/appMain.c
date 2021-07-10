/*!

\author         Oliver Blaser
\date           10.07.2021
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
    S_setDispAt,
    S_setIO,
    S_getIO,
    _S_last
} state_t;


int setupUDC();


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
            if(!setupUDC()) state = S_idle;
            break;
            
        case S_idle:
            {
                uint8_t cmd = COM_APP_cmd();
                
                if(cmd == COM_CMD_SETDISP) state = S_setDisp;
                else if(cmd == COM_CMD_SETDISPAT) state = S_setDispAt;
                else if(cmd == COM_CMD_SETIO) state = S_setIO;
                else if(cmd == COM_CMD_GETIO) state = S_getIO;
                else if(cmd != COM_CMD_NONE) *ts |= (TASK_TAPP | TASK_INVCMD);
            }
            break;
            
        case S_setDisp:
            {
                const uint8_t* buffer;
                size_t count;
                COM_APP_getDispPtr(&buffer, &count);
                HDSP_printAt((const char*)buffer, 0, count);
                HDSP_printAt("        ", count, 8);
                COM_APP_sendOK();
                state = S_idle;
            }
            break;
            
        case S_setDispAt:
            {
                size_t len;
                const uint8_t* buffer = COM_getData(&len);
                HDSP_printAt(&buffer[1], buffer[0], len - 1);
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



// WDT safe
int setupUDC()
{
    static uint8_t index = 0;
    
    uint8_t udc[7];
    
    if(index == 0)
    {
        // garbage
        for(uint8_t i = 0; i < 7; ++i) udc[i] = i;
    }
    else if(index == 1)
    {
        // battery 0%
        udc[0] = 0b01110;
        udc[1] = 0b11111;
        udc[2] = 0b10001;
        udc[3] = 0b10001;
        udc[4] = 0b10001;
        udc[5] = 0b10001;
        udc[6] = 0b11111;
    }
    else if(index == 2)
    {
        // battery 25%
        udc[0] = 0b01110;
        udc[1] = 0b11111;
        udc[2] = 0b10001;
        udc[3] = 0b10001;
        udc[4] = 0b10001;
        udc[5] = 0b11111;
        udc[6] = 0b11111;
    }
    else if(index == 3)
    {
        // battery 50%
        udc[0] = 0b01110;
        udc[1] = 0b11111;
        udc[2] = 0b10001;
        udc[3] = 0b10001;
        udc[4] = 0b11111;
        udc[5] = 0b11111;
        udc[6] = 0b11111;
    }
    else if(index == 4)
    {
        // battery 75%
        udc[0] = 0b01110;
        udc[1] = 0b11111;
        udc[2] = 0b10001;
        udc[3] = 0b11111;
        udc[4] = 0b11111;
        udc[5] = 0b11111;
        udc[6] = 0b11111;
    }
    else if(index == 5)
    {
        // battery 100%
        udc[0] = 0b01110;
        udc[1] = 0b11111;
        udc[2] = 0b11111;
        udc[3] = 0b11111;
        udc[4] = 0b11111;
        udc[5] = 0b11111;
        udc[6] = 0b11111;
    }
    else if(index == 6)
    {
        // smiley
        udc[0] = 0;
        udc[1] = 0b01010;
        udc[2] = 0b01010;
        udc[3] = 0;
        udc[4] = 0b10001;
        udc[5] = 0b01110;
        udc[6] = 0;
    }
    else if(index == 7)
    {
        // circumflex accent
        udc[0] = 0;
        udc[1] = 0b00100;
        udc[2] = 0b01010;
        udc[3] = 0b10001;
        udc[4] = 0;
        udc[5] = 0;
        udc[6] = 0;
    }
    else if(index == 8)
    {
        // light shade
        udc[0] = 0b10001;
        udc[1] = 0b00100;
        udc[2] = udc[0];
        udc[3] = udc[1];
        udc[4] = udc[0];
        udc[5] = udc[1];
        udc[6] = udc[0];
    }
    else if(index == 9)
    {
        // dark shade
        udc[0] = 0b11011;
        udc[1] = 0b01110;
        udc[2] = udc[0];
        udc[3] = udc[1];
        udc[4] = udc[0];
        udc[5] = udc[1];
        udc[6] = udc[0];
    }
    else if(index == 10)
    {
        // full block
        for(uint8_t i = 0; i < 7; ++i) udc[i] = 0b11111;
    }
    else if(index == 11)
    {
        // triangle right
        udc[0] = 0b01000;
        udc[1] = 0b01100;
        udc[2] = 0b01110;
        udc[3] = 0b01111;
        udc[4] = 0b01110;
        udc[5] = 0b01100;
        udc[6] = 0b01000;
    }
    else if(index == 12)
    {
        // vertical line
        for(uint8_t i = 0; i < 7; ++i) udc[i] = 0b00100;
    }
    else return 0;
    
    HDSP_setUDC(index, udc);
    
    ++index;
    return 1;
}
