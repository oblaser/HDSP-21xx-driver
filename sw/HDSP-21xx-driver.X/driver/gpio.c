/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "gpio.h"

void GPIO_init()
{
    ANSEL   = 0b00000000;
    ANSELH  = 0b00000000;
    
    PORTA = 0;
    TRISA   = 0b00111111;
    WPUA    = 0b00000000;
    IOCA    = 0b00000000;
    
    PORTB = 0;
    TRISB   = 0b11100000;
    WPUB    = 0b00000000;
    IOCB    = 0b00000000;
    
    PORTC = 0;
    TRISC   = 0b11111111;
    
#if(PRJ_PROGIO_IN)
    WPUAbits.WPUA4 = 1;
    TRISAbits.TRISA4 = 1;
#endif
#if(PRJ_PROGIO_OUT)
    TRISAbits.TRISA4 = 0;
#endif
    
    // to have a valid inpOld
    GPIO_inpDetect();
}

GPIO_input_t GPIO_inpDetect()
{
    static GPIO_inpType_t inpOld = 0;
    GPIO_input_t r;
    r.state = 0;
    
#if(PRJ_PROGIO_IN)
    if (GPIO_PROGIO) r.state |= GPIO_INP_PROGIO;
#endif
    
    if(GPIO_BUTTON) r.state |= GPIO_INP_BUTTON;
    
    r.rising = ~inpOld & r.state;
    r.falling = inpOld & ~r.state;
    
    inpOld = r.state;
    
    return r;
}

void GPIO_dispData_write(uint8_t data)
{
    GPIO_dispData_R_nW(0);
    
    PORTC = (data & 0x3F);
    
    if(data & 0x80) PORTAbits.RA5 = 1; 
    else PORTAbits.RA5 = 0;
}

uint8_t GPIO_dispData_read()
{
    GPIO_dispData_R_nW(1);
    
    uint8_t r = (PORTC & 0x3F);
    
    if(PORTAbits.RA5) r |= 0x80;
    
    return r;
}

void GPIO_dispData_R_nW(int R_nW)
{
    if(R_nW)
    {
        TRISC |= 0x3F;
        TRISAbits.TRISA5 = 1;
    }
    else
    {
        TRISC &= ~0x3F;
        TRISAbits.TRISA5 = 0;
    }
}
