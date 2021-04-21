/*!

\author         Oliver Blaser
\date           21.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _UTIL_H_
#define _UTIL_H_


#include "../types.h"


#define UTIL_CONFIG_USE_ITOA_BUFFER (1)

#define UTIL_STRERR_INVPTR ((int)-1)
#define UTIL_STRERR_BUFSIZE ((int)-2)


size_t UTIL_strnlen(const char* str, size_t size);

int UTIL_itoa(int32_t value, char* buffer, size_t bufferSize);

#if(UTIL_CONFIG_USE_ITOA_BUFFER)
const char* UTIL_itoap(int32_t value);
#endif


#endif // _UTIL_H_
