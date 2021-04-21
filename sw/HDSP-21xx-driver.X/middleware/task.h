/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _TASK_H_
#define _TASK_H_


#include "../types.h"


#define TASK_OK ((TASK_status_t)0)

#define TASK_TAPP ((TASK_status_t)0x80)
#define TASK_TCOM ((TASK_status_t)0x40)

#define TASK_ERR ((TASK_status_t)0x01)
#define TASK_STATE ((TASK_status_t)0x02)
#define TASK_TIMEOUT ((TASK_status_t)0x04)


typedef uint8_t TASK_status_t;


#endif // _TASK_H_
