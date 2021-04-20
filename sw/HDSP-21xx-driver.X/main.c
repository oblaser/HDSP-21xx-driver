/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "application/appMain.h"
#include "driver/hardware_16F690.h"
#include "driver/gpio.h"
#include "driver/spi.h"
#include "driver/uart.h"
#include "middleware/com.h"
#include "middleware/task.h"
#include "middleware/util.h"



inline void errorHandler(const TASK_status_t* ts);
inline void TMR0_isr();

int main()
{
    static TASK_status_t ts;
    
    HW_init();
    HW_wdtEnable();
    
    GPIO_init();
    SPI_init();
    UART_init();
    
    HW_gieSet();
    
    UART_blocking_print("=====\n\nstarted...\n");
    
    static int cnt = 0;
    while(1)
    {
        GPIO_input_t inp;
        GPIO_inpDetect(&inp);
        if(inp.rising & GPIO_INP_BUTTON) 
        {
#if 1
            uint8_t d[2] = { (0x0F | (cnt << 4)), 0b11011111 };
            
            GPIO_PROGIO = 1;
            SPI_blocking_write(d, 2);
            //SPI_write(d, 2);
            GPIO_PROGIO = 0;
            
            // @8MHz: creates a 500ns pulse
            GPIO_SHIFTREG_LATCH = 1;
            GPIO_SHIFTREG_LATCH = 0;
            
            ++cnt;
            
            UART_blocking_print("cnt: ");
            UART_blocking_print(UTIL_itoap(cnt & 0x0F));
            UART_blocking_print("\n");
#else
            uint8_t d[20];
            for(int i=0;i<20;++i) d[i] = i;
            
#if 0
            GPIO_PROGIO = 1;
            SPI_write(d, 10);
            GPIO_PROGIO = 0;
            
            while(SPI_write(0, 0) == SPI_WRITE_BUSY) asm("NOP");
            
            GPIO_PROGIO = 1;
            GPIO_PROGIO = 0;
#else
            int a = SPI_write(d, 20);
            int b = SPI_write(d, 20);
            UART_blocking_print("SPI_write: ");
            UART_blocking_print(UTIL_itoap(a));
            UART_blocking_print(UTIL_itoap(b));
            UART_blocking_print("\n");
#endif
#endif
        }
        
        ts = TASK_OK;
        
        APP_task(&ts);
        COM_task(&ts);
        
        errorHandler(&ts);
        
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
    
    if(SSPIE && SSPIF) SPI_isr();
    
    if(RCIE && RCIF) UART_rx_isr();
    if(TXIE && TXIF) UART_tx_isr();
}



inline void errorHandler(const TASK_status_t* ts)
{
    if(*ts)
    {
        UART_blocking_print("task error: ");
        UART_blocking_print(UTIL_itoap(*ts));
        UART_blocking_print("\n");
    }
}

inline void TMR0_isr()
{
    APP_timeHandler();
    COM_timeHandler();
}
