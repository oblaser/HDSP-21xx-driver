/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../driver/gpio.h"
#include "hdsp_21xx.h"
#include "shiftReg.h"



void write_char(size_t digit, char c);



void HDSP_init()
{
    SHR_DISP_reset();
}

void HDSP_print(const char* str)
{
    if(str)
    {
        size_t n = 0;
        while((*(str + n) != 0) && (n < HDSP_nDIGITS)) ++n;
        
        for(size_t i = 0; i < HDSP_nDIGITS; ++i)
        {
            if(i < n) write_char(i, str[i]);
            else write_char(i, 0x20);
        }
    }
}



// 450us (SSPM=0b0001 / 8MHz / -O2)
void write_char(size_t digit, char c)
{
    SHR_DISP_setAddr(0x0038 | ((uint16_t)digit & 0x0007));
    SHR_DISP_nChipEnable(0);
    GPIO_DISP_R_nW(0);
    GPIO_DISP_setData((uint8_t)c);
    SHR_DISP_write();
    GPIO_DISP_R_nW(1);
    SHR_DISP_nChipEnable(1);
}
