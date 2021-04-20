/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _SHIFTREG_H_
#define _SHIFTREG_H_

#include "../types.h"
#include "task.h"



#define SHR_MASK_ADDR   ((uint16_t)0x001F)
#define SHR_MASK_ADDR0  ((uint16_t)0x0001)
#define SHR_MASK_ADDR1  ((uint16_t)0x0002)
#define SHR_MASK_ADDR2  ((uint16_t)0x0004)
#define SHR_MASK_ADDR3  ((uint16_t)0x0008)
#define SHR_MASK_ADDR4  ((uint16_t)0x0010)
#define SHR_MASK_nRST   ((uint16_t)0x0020)
#define SHR_MASK_nFL    ((uint16_t)0x0040)
#define SHR_MASK_CLS    ((uint16_t)0x0080)
#define SHR_MASK_nWR    ((uint16_t)0x0200)
#define SHR_MASK_nRD    ((uint16_t)0x0400)
#define SHR_MASK_nCE    ((uint16_t)0x0800)
#define SHR_MASK_LED    ((uint16_t)0xF000)
#define SHR_MASK_LED1   ((uint16_t)0x1000)
#define SHR_MASK_LED2   ((uint16_t)0x2000)
#define SHR_MASK_LED3   ((uint16_t)0x4000)
#define SHR_MASK_LED4   ((uint16_t)0x8000)



void SHR_init();

void SHR_modify(uint16_t set, uint16_t clr);

void SHR_LED_set(uint16_t led);
uint8_t SHR_LED_get();

void SHR_DISP_reset();
//void SHR_DISP_read();
void SHR_DISP_write();
void SHR_DISP_setAddr(uint16_t addr);
void SHR_DISP_nChipEnable(uint8_t nCE);

#endif // _SHIFTREG_H_
