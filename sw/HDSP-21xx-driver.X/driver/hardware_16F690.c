/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../project.h"
#include "hardware_16F690.h"

#include <xc.h>
#include <pic16f690.h>

#if (PRJ_PROGIO_CLKOUT)
#pragma config FOSC = INTRCCLK  // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#else
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#endif
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

void HW_init()
{
    //--------------------------------
    // 3.0 Clock Sources
    //--------------------------------
    
    OSCCON  = 0b01110001;   // internal 8MHz
    OSCTUNE = 0b00000000;
    
    //--------------------------------
    // 4.0 I/O PORTs
    //--------------------------------
    
    // gpio.h -> GPIO_init()
    
    //--------------------------------
    // 5.0 Timer0
    //--------------------------------
    
    OPTION_REG = 0b01010110;    // CS = Fosc/4 PS = 1:128
    TMR0       = 0b00000000;
    
    //--------------------------------
    // 6.0 Timer1
    //--------------------------------
    
    // disabled by default, no changes needed
    
    //--------------------------------
    // 7.0 Timer2
    //--------------------------------
    
    // disabled by default, no changes needed
    
    //--------------------------------
    // 8.0 Comparator
    //--------------------------------
    
    // disabled by default, no changes needed
    
    //--------------------------------
    // 9.0 ADC
    //--------------------------------
    
    // disabled by default, no changes needed
    
    //--------------------------------
    // 10.0 EEPROM
    //--------------------------------
    
    // no changes needed
    
    //--------------------------------
    // 11.0 ECCP+
    //--------------------------------
    
    // disabled by default, no changes needed
    
    //--------------------------------
    // 12.0 EUSART
    //--------------------------------
    
    // uart.h -> UART_init()
    
    //--------------------------------
    // 13.0 SSP
    //--------------------------------
    
    // spi.h -> SPI_init()
    
    //--------------------------------
    // 14.3 Interrupt
    //--------------------------------
    
    INTCON  = 0b01100000;    // Peripheral Timer0
    PIE1    = 0b00000000;
    PIE2    = 0b00000000;
    
    //--------------------------------
    // 14.5 WDT
    //--------------------------------
    
    WDTCON = 0b00001010;    // 33ms timeout 
}

void HW_gieSet()
{
    GIE = 1;
}

void HW_gieClr()
{
    GIE = 0;
}

void HW_wdtEnable()
{
    SWDTEN = 1;
}

void HW_wdtDisable()
{
    SWDTEN = 0;
}

void HW_wdtClr()
{
    asm("CLRWDT");
}
