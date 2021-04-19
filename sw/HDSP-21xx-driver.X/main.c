/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "driver/hardware_16F690.h"
#include "driver/gpio.h"
#include "driver/uart.h"


void TMR0_isr();

static int tmr_led = 0;



int main()
{
    HW_init();
    HW_wdtEnable();
    
    GPIO_init();
    
    HW_gieSet();
    
    while(1)
    {
        GPIO_input_t inp = GPIO_inpDetect();
        
        if(inp.rising & GPIO_INP_BUTTON) tmr_led = 0;
        
        if(tmr_led == 0)
        {
            tmr_led = 100;
            
            GPIO_PROGIO ^= 1;
        }
        
        HW_wdtClr();
    }
    
    return 0;
}

void __interrupt() global_isr()
{
    // ~10ms (9.984ms)
    if(T0IE && T0IF)
    {
        TMR0_isr();
        
        TMR0 = 100;
        T0IF = 0;
    }
}



void TMR0_isr()
{
    if(tmr_led > 0) --tmr_led;
}
