/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "../types.h"
#include "com.h"



void calcCheckSum(uint8_t* cs, const uint8_t* data, size_t count);

static uint8_t rxBuffer[11];



void COM_task(TASK_status_t* ts)
{
}

void COM_timeHandler()
{
}

void COM_sendTaskError(const TASK_status_t* ts)
{
    
}



void calcCheckSum(uint8_t* cs, const uint8_t* data, size_t count)
{
    *cs = *data;
    for(size_t i = 1; i < count; ++i) *cs ^= *(data + i);
}
