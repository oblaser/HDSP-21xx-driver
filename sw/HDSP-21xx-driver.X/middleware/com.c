/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include <stdint.h>

#include "com.h"



void calcCheckSum(uint8_t* cs, const uint8_t* data, unsigned count);

static uint8_t rxBuffer[11];



void COM_task(TASK_status_t* ts)
{
}

void COM_timeHandler()
{
}



void calcCheckSum(uint8_t* cs, const uint8_t* data, unsigned count)
{
    *cs = 0;
    for(unsigned i = 0; i < count; ++i) *cs ^= *(data + i);
}
