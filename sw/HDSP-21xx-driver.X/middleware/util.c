/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#include "util.h"

#define _UTIL_ITOA_BUFFERSIZE (12)

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
