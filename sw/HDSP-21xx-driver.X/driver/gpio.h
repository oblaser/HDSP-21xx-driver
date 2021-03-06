/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include "../types.h"
#include "../project.h"
#include "hardware_16F690.h"

#define GPIO_BUTTON PORTAbits.RA2
#define GPIO_SHR_LATCH PORTBbits.RB4
#if(PRJ_PROGIO_IN || PRJ_PROGIO_OUT)
#define GPIO_PROGIO PORTAbits.RA4
#endif

#define GPIO_INP_BUTTON ((GPIO_inpType_t)0x02)
#if(PRJ_PROGIO_IN)
#define GPIO_INP_PROGIO ((GPIO_inpType_t)0x01)
#endif

typedef uint8_t GPIO_inpType_t;
typedef struct
{
    GPIO_inpType_t state;   // current state
    GPIO_inpType_t rising;  // rising edge
    GPIO_inpType_t falling; // falling edge
} GPIO_input_t;

void GPIO_init();

void GPIO_inpDetect(GPIO_input_t* inp);

void GPIO_DISP_setData(uint8_t data);
uint8_t GPIO_DISP_getData();
void GPIO_DISP_R_nW(uint8_t R_nW);

#endif // _GPIO_H_
