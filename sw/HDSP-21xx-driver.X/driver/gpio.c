/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "gpio.h"


#define PORTC_DISP_MASK (0x7F)

void readInp(GPIO_inpType_t* inp);



void GPIO_init()
{
    ANSEL   = 0b00000000;
    ANSELH  = 0b00000000;
    
    PORTA = 0;
    TRISA   = 0b00111111;
    WPUA    = 0b00000000;
    IOCA    = 0b00000000;
    
    PORTB = 0;
    TRISB   = 0b10100000;
    WPUB    = 0b00000000;
    IOCB    = 0b00000000;
    
    PORTC = 0;
    TRISC   = 0b01111111;
    
#if(PRJ_PROGIO_IN)
    WPUAbits.WPUA4 = 1;
    TRISAbits.TRISA4 = 1;
#endif
#if(PRJ_PROGIO_OUT)
    TRISAbits.TRISA4 = 0;
#endif
    
    // to have a valid inpOld
    GPIO_inpDetect(0);
}

void GPIO_inpDetect(GPIO_input_t* inp)
{
    static GPIO_inpType_t inpOld = 0;
    
    if(inp)
    {
        readInp(&(inp->state));
        
        inp->rising = ~inpOld & inp->state;
        inp->falling = inpOld & ~inp->state;

        inpOld = inp->state;
    }
    else readInp(&inpOld);
}

void GPIO_DISP_setData(uint8_t data)
{
    if(((TRISC & PORTC_DISP_MASK) == 0) && (TRISAbits.TRISA5 == 0))
    {
        PORTC = (data & PORTC_DISP_MASK);

        if(data & 0x80) PORTAbits.RA5 = 1; 
        else PORTAbits.RA5 = 0;
    }
}

uint8_t GPIO_DISP_getData()
{
    uint8_t r = (PORTC & PORTC_DISP_MASK);
    
    if(PORTAbits.RA5) r |= 0x80;
    
    return r;
}

void GPIO_DISP_R_nW(uint8_t R_nW)
{
    if(R_nW)
    {
        TRISC |= PORTC_DISP_MASK;
        TRISAbits.TRISA5 = 1;
    }
    else
    {
        TRISC &= ~PORTC_DISP_MASK;
        TRISAbits.TRISA5 = 0;
    }
}



void readInp(GPIO_inpType_t* inp)
{
    if(inp)
    {
        *inp = 0;
        
#if(PRJ_PROGIO_IN)
        if (GPIO_PROGIO) *inp |= GPIO_INP_PROGIO;
#endif

        if(GPIO_BUTTON) *inp |= GPIO_INP_BUTTON;
    }
}
