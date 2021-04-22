/*!

\author         Oliver Blaser
\date           21.04.2021
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
#include "middleware/hdsp_21xx.h"



void errorHandler(const TASK_status_t* ts);
void TMR0_isr();

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
    
#if PRJ_HWTEST
    static int cnt = 0;
    static char hwTestStr[12];
    
    HDSP_init();
    
    UART_print("\n-===# HW TEST #===-\n");
    HDSP_print("HDSP2113");
    
    while(1)
    {
        HW_wdtClr();
        
        GPIO_input_t inp;
        GPIO_inpDetect(&inp);
        
        if(inp.rising & GPIO_INP_BUTTON)
        {
            if(cnt == 0)
            {
                HDSP_printAt("\x0B\x0C", 1, 2);
                HDSP_printAt("wxyz", 4, 4);
            }
            else
            {
                UTIL_itoa((cnt % 2 ? cnt : (0 - cnt)), hwTestStr, (sizeof(hwTestStr)/sizeof(hwTestStr[0])));
                size_t len = UTIL_strnlen(hwTestStr, (sizeof(hwTestStr)/sizeof(hwTestStr[0])));

                HDSP_print("        ");
                HDSP_printAt(hwTestStr, (HDSP_nDIGITS - len), len);

                SHR_LED_set(cnt);

                UART_print_wait(hwTestStr);
                UART_print_wait("   ");
                UART_print_wait(UTIL_itoap(HDSP_nDIGITS - len));
                UART_print_wait("   ");
                UART_print_wait(UTIL_itoap(len));
                UART_print_wait("\n");
            }
            
            cnt += 3;
        }
    }
#endif
    
#if (PRJ_DEBUG_UART)
    UART_print_blocking("\n-===# started #===-\n");
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



void errorHandler(const TASK_status_t* ts)
{
    if(*ts)
    {
        COM_sendTaskError(ts);
        
#if PRJ_DEBUG_UART
        UART_print_wait("task error: ");
        UART_print_wait(UTIL_itoap(*ts));
        UART_print_wait("\n");
#endif
        
        HW_wdtClr();
        while(1) asm("NOP");
    }
}

void TMR0_isr()
{
    APP_timeHandler();
    COM_timeHandler();
}
