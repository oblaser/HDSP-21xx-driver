/*!

\author         Oliver Blaser
\date           21.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _UTIL_H_
#define _UTIL_H_


#include "../types.h"


#define UTIL_CONFIG_USE_ITOA_BUFFER (1)

//! Returned if a pointer is null
#define UTIL_STRERR_INVPTR ((int)-1)

//! Returned if the destination buffer is too small
#define UTIL_STRERR_BUFSIZE ((int)-2)


size_t UTIL_strnlen(const char* str, size_t size);

int UTIL_itoa(int32_t value, char* str, size_t strSize);
#if(UTIL_CONFIG_USE_ITOA_BUFFER)
const char* UTIL_itoap(int32_t value);
#endif

size_t UTIL_byteToHexStr(uint8_t byte, char* str, size_t strSize);
size_t UTIL_bufToHexStr(const uint8_t* buffer, size_t count, char* str, size_t strSize);


#endif // _UTIL_H_
