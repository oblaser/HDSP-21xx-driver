/*!

\author         Oliver Blaser
\date           21.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../driver/gpio.h"
#include "../middleware/hdsp_21xx.h"
#include "../middleware/shiftReg.h"
#include "../types.h"
#include "appMain.h"


typedef enum
{
    S_init = 0,
    S_idle,
} state_t;




void APP_task(TASK_status_t* ts)
{
    static state_t state = S_init;
    
    GPIO_input_t inp;
    GPIO_inpDetect(&inp);
    
    switch(state)
    {
        case S_init:
            HDSP_init();
            state = S_idle;
            break;
            
        case S_idle:
            
            break;
            
        default:
            *ts |= (TASK_TAPP | TASK_STATE);
            break;
    }
}

void APP_timeHandler()
{
}
