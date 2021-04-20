/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _COM_H_
#define _COM_H_

#include "../middleware/task.h"

void COM_task(TASK_status_t* ts);

void COM_timeHandler();

void COM_sendTaskError(const TASK_status_t* ts);

#endif // _COM_H_
