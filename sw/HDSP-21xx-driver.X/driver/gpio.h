/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

#include "../project.h"
#include "hardware_16F690.h"

#define GPIO_BUTTON PORTAbits.RA2
#define GPIO_SHIFTREG_LATCH PORTBbits.RB4
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

GPIO_input_t GPIO_inpDetect();

void GPIO_dispData_write(uint8_t data);
uint8_t GPIO_dispData_read();
void GPIO_dispData_R_nW(int R_nW);

#endif // _GPIO_H_
