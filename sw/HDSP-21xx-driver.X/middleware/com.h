/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _COM_H_
#define _COM_H_

#include "../middleware/task.h"

void COM_task(TASK_status_t* ts);

void COM_timeHandler();

#endif // _COM_H_
