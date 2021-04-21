/*!

\author         Oliver Blaser
\date           21.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "util.h"


#define _UTIL_ITOA_BUFFERSIZE (12)


size_t UTIL_strnlen(const char* str, size_t size)
{
    size_t len = 0;
    if(str) while((*(str + len) != 0) && (len < size)) ++len;
    return len;
}

int UTIL_itoa(int32_t value, char* buffer, size_t bufferSize)
{
    char revBuf[_UTIL_ITOA_BUFFERSIZE];
    int revBufIndex = 0;
    uint32_t val = *(&value);
    char* p = buffer;
    
    if(!buffer) return UTIL_STRERR_INVPTR;
    if(bufferSize < _UTIL_ITOA_BUFFERSIZE) return UTIL_STRERR_BUFSIZE;
    
    if(value == 0)
    {
        *buffer = '0';
        *(buffer + 1) = 0;
        return 1;
    }
    
    if(value < 0)
    {
        *p = '-';
        ++p;
        
        val = ~val;
        ++val;
    }
    
    while(val != 0)
    {
        revBuf[revBufIndex] = (val % 10) + 0x30;
        ++revBufIndex;
        val /= 10;
    }
    
    --revBufIndex;
    
    while(revBufIndex > (-1))
    {
        *p = revBuf[revBufIndex];
        ++p;
        --revBufIndex;
    }
    
    *p = 0;
    
    return (p - buffer);
}

#if(UTIL_CONFIG_USE_ITOA_BUFFER)
static char itoa_buffer[_UTIL_ITOA_BUFFERSIZE];
const char* UTIL_itoap(int32_t value)
{
    if(UTIL_itoa(value, itoa_buffer, (sizeof(itoa_buffer)/sizeof(itoa_buffer[0]))) < 0) return 0;
    return itoa_buffer;
}
#endif
