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
#include "middleware/shiftReg.h"
#include "middleware/task.h"
#include "middleware/util.h"



inline void errorHandler(const TASK_status_t* ts);
inline void TMR0_isr();

int main()
{
    static TASK_status_t ts = TASK_OK;
    
    HW_init();
    HW_wdtEnable();
    
    GPIO_init();
    UART_init();
    SPI_init();
    SHR_init();
    
    HW_gieSet();
    
#if PRJ_PROGIO_OUT
    GPIO_PROGIO = 0;
    for(int i = 0; i < 10; ++i) HW_wdtClr();
    GPIO_PROGIO = 1;
    for(int i = 0; i < 10; ++i) HW_wdtClr();
    GPIO_PROGIO = 0;
#endif
    
#if PRJ_DEBUG_UART
    UART_print("=====\n\nstarted...\n");
#endif
    
    while(1)
    {
        HW_wdtClr();
        
        errorHandler(&ts);
        
        ts = TASK_OK;
        
        APP_task(&ts);
        COM_task(&ts);
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



inline void errorHandler(const TASK_status_t* ts)
{
    if(*ts)
    {
        COM_sendTaskError(ts);
        
#if PRJ_DEBUG_UART
        UART_blocking_print("task error: ");
        UART_blocking_print(UTIL_itoap(*ts));
        UART_blocking_print("\n");
#endif
        
        while(1) asm("NOP");
    }
}

inline void TMR0_isr()
{
    APP_timeHandler();
    COM_timeHandler();
}
