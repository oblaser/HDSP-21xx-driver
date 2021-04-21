/*!

\author         Oliver Blaser
\date           21.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _COM_H_
#define _COM_H_


#include "../middleware/task.h"
#include "../types.h"


#define COM_CMD_NONE    ((uint8_t)0x00)
#define COM_CMD_GETINFO ((uint8_t)0x01)
#define COM_CMD_SETDISP ((uint8_t)0x0A)
#define COM_CMD_SETIO   ((uint8_t)0x10)
#define COM_CMD_GETIO   ((uint8_t)0x11)


void COM_task(TASK_status_t* ts);
void COM_timeHandler();

uint8_t COM_APP_cmd();
const uint8_t* COM_APP_getDispPtr();
uint8_t COM_APP_getIoData();
void COM_APP_sendOK();
void COM_APP_sendGetIO(uint8_t ioState);

void COM_sendTaskError(const TASK_status_t* ts);

#endif // _COM_H_
