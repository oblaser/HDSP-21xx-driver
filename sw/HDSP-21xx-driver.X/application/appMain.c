/*!

\author         Oliver Blaser
\date           30.03.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../driver/gpio.h"
#include "../middleware/hdsp_21xx.h"
#include "../middleware/shiftReg.h"
#include "../types.h"
#include "appMain.h"

typedef enum
{
    S_init,
    S_idle,
} state_t;

static uint8_t cnt = 0;
static char dispStr[9] = "ABCDEFGH";


void APP_task(TASK_status_t* ts)
{
    static state_t state = S_init;
    
    GPIO_input_t inp;
    GPIO_inpDetect(&inp);
    
    switch(state)
    {
        case S_init:
            
            HDSP_init();
            
            HDSP_print(dispStr);
            dispStr[0] = 5;
            dispStr[1] = 11;
            dispStr[2] = 12;
            dispStr[3] = 13;
            dispStr[4] = 14;
            dispStr[5] = 0;
            
            state = S_idle;
            
            break;
            
        case S_idle:
            
            if(inp.rising & GPIO_INP_BUTTON)
            {
                HDSP_print(dispStr);
                
                SHR_LED_set(++cnt);
            }
            
            break;
            
        default:
            *ts |= (TASK_TAPP | TASK_ERR);
            break;
    }
}

void APP_timeHandler()
{
}
