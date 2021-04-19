/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "application/appMain.h"
#include "driver/hardware_16F690.h"
#include "driver/gpio.h"
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
    UART_init();
    
    HW_gieSet();
    
    while(1)
    {
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
    
    if(RCIE && RCIF) UART_rx_isr();
    if(TXIE && TXIF) UART_tx_isr();
}



inline void errorHandler(const TASK_status_t* ts)
{
    if(*ts)
    {
        char tmpStr[12];
        UART_blocking_print("task error: ");
        UTIL_itoa(*ts, tmpStr, (sizeof(tmpStr)/sizeof(tmpStr[0])));
        UART_blocking_print(tmpStr);
        UART_blocking_print("\n");
    }
}

inline void TMR0_isr()
{
    APP_timeHandler();
    COM_timeHandler();
}
