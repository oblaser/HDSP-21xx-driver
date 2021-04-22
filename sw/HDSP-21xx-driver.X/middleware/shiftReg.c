/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../driver/gpio.h"
#include "../driver/spi.h"
#include "shiftReg.h"



void send();

static uint16_t dataReg = (SHR_MASK_CLS | SHR_MASK_nWR | SHR_MASK_nRD | SHR_MASK_nCE);



void SHR_init()
{
    dataReg = 0;
    
    dataReg |= SHR_MASK_CLS;
    dataReg |= SHR_MASK_nWR;
    dataReg |= SHR_MASK_nRD;
    dataReg |= SHR_MASK_nCE;
    
    send();
}

void SHR_modify(uint16_t set, uint16_t clr)
{
    dataReg |= set;
    dataReg &= ~clr;
    
    send();
}

void SHR_LED_set(uint16_t led)
{
    led <<= 12;
    
    dataReg &= ~SHR_MASK_LED;
    dataReg |= (led & SHR_MASK_LED);
    
    send();
}

uint8_t SHR_LED_get()
{
    return (uint8_t)((dataReg >> 12) & 0x000F);
}

void SHR_DISP_reset()
{
    dataReg &= ~SHR_MASK_nRST;
    send();
    dataReg |= SHR_MASK_nRST;
    send();
}

void SHR_DISP_nWrite(uint8_t nWR)
{
    if(nWR) dataReg |= SHR_MASK_nWR;
    else dataReg &= ~SHR_MASK_nWR;
    send();
}

void SHR_DISP_setAddr(uint16_t addr)
{
    if(addr & 0xFFE0) dataReg |= SHR_MASK_nFL;
    else dataReg &= ~SHR_MASK_nFL;
    
    dataReg &= ~SHR_MASK_ADDR;
    dataReg |= (addr & SHR_MASK_ADDR);
    
    send();
}

void SHR_DISP_nChipEnable(uint8_t nCE)
{
    if(nCE) dataReg |= SHR_MASK_nCE;
    else dataReg &= ~SHR_MASK_nCE;
    
    send();
}



void send()
{
    uint8_t data[2];
    
    data[0] = (uint8_t)((dataReg >> 8) & 0x00FF);
    data[1] = (uint8_t)(dataReg & 0x00FF);
    
    SPI_write_blocking(data, 2);
    
    GPIO_SHR_LATCH = 1;
    GPIO_SHR_LATCH = 0;
}
