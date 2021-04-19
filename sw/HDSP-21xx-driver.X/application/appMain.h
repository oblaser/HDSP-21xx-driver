/*!

\author         Oliver Blaser
\date           30.03.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _APPMAIN_H_
#define	_APPMAIN_H_

#include "../middleware/task.h"

void APP_task(TASK_status_t* ts);

void APP_timeHandler();

#endif // _APPMAIN_H_
