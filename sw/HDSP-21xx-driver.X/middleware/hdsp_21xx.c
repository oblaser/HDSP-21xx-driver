/*!

\author         Oliver Blaser
\date           21.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../driver/gpio.h"
#include "hdsp_21xx.h"
#include "shiftReg.h"

#include "../driver/uart.h"
#include "util.h"



void write_char(size_t digit, char c);



void HDSP_init()
{
    SHR_DISP_reset();
}

// speed measurement in 1a379faed394999b486cfc6947dc504c544bbe0f
void HDSP_write(uint8_t addr, uint8_t data)
{
    SHR_DISP_setAddr(addr);
    SHR_DISP_nChipEnable(0);
    GPIO_DISP_R_nW(0);
    SHR_DISP_nWrite(0);
    GPIO_DISP_setData(data);
    SHR_DISP_nWrite(1);
    GPIO_DISP_R_nW(1);
    SHR_DISP_nChipEnable(1);
}

void HDSP_setUDC(uint8_t index, const uint8_t* matrix)
{
    if(matrix)
    {
        HDSP_write(0x20, index);
        for(uint8_t i = 0; i < 7; ++i) HDSP_write(0x28 | i, matrix[i]);
    }
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

void HDSP_printAt(const char* str, size_t pos, size_t count)
{
    if(str)
    {
        size_t i = pos;
        while(((i - pos) < count) && (i < HDSP_nDIGITS))
        {
            write_char(i, str[i - pos]);
            ++i;
        }
    }
}



void write_char(size_t digit, char c)
{
    HDSP_write(0x38 | ((uint8_t)digit & 0x07), (uint8_t)c);
}
