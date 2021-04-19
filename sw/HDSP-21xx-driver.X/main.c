/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "driver/hardware_16F690.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "middleware/util.h"


void TMR0_isr();

static int tmr_led = 0;

void test_itoa(int32_t value, int expectedLen)
{
    char tmpStr[12];
    int len = UTIL_itoa(value, tmpStr, (sizeof(tmpStr)/sizeof(tmpStr[0])));
    
    UART_blocking_print(tmpStr);
    
    if(len == expectedLen) UART_blocking_print(" - OK\n");
    else UART_blocking_print(" - failed\n");
}

int main()
{
    HW_init();
    //HW_wdtEnable();
    
    GPIO_init();
    UART_init();
    
    HW_gieSet();
    
    UART_print("\nStartup done\n");
    
    test_itoa(0, 1);
    test_itoa(sizeof(size_t), 1);
    test_itoa(123, 3);
    test_itoa(-123, 4);
    test_itoa(0x7FFFFFFF, 10);
    test_itoa(0x80000000, 11);
    
    static GPIO_input_t inp;
        
    while(1)
    {
        GPIO_inpDetect(&inp);
        
#if(PRJ_PROGIO_IN)
        if(inp.rising & GPIO_INP_PROGIO)
        {
            UART_reset();
            UART_print("UART resetted\n");
        }
#endif
        
        if(inp.rising & GPIO_INP_BUTTON)
        {
            int rxByte = UART_read();
            
            if(rxByte == UART_READ_NODATA) UART_print("no data\n");
            else if(rxByte == UART_READ_BUFFEROVERFLOW) UART_print("rxBuffer overflow\n");
            else
            {
                char tmpStr[] = "#\n";
                tmpStr[0] = (char)rxByte;
                UART_print(tmpStr);
            }
        }
      
#if(PRJ_PROGIO_OUT)
        if(tmr_led == 0)
        {
            tmr_led = 50;
            
            GPIO_PROGIO ^= 1;
        }
#endif
        
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
    
    if(RCIE && RCIF) UART_rx_isr();
    if(TXIE && TXIF) UART_tx_isr();
}



void TMR0_isr()
{
    if(tmr_led > 0) --tmr_led;
}
