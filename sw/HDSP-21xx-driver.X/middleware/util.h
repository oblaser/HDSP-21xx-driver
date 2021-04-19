/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <string.h>


#define UTIL_STRERR_INVPTR ((int)-1)
#define UTIL_STRERR_BUFSIZE ((int)-2)


int UTIL_itoa(int32_t value, char* buffer, size_t bufferSize);

#endif // _UTIL_H_
