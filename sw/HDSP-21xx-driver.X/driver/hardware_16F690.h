/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _HARDWARE_16F690_H_
#define _HARDWARE_16F690_H_

#include <xc.h>
#include <pic16f690.h>

void HW_init();

void HW_gieSet();
void HW_gieClr();

void HW_wdtEnable();
void HW_wdtDisable();
void HW_wdtClr();

#endif // _HARDWARE_16F690_H_
