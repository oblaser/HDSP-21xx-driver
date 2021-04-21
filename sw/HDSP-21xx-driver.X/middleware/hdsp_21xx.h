/*!

\author         Oliver Blaser
\date           20.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _HDSP_21XX_H_
#define _HDSP_21XX_H_

#include "../types.h"


#define HDSP_nDIGITS ((size_t)8)


void HDSP_init();

void HDSP_print(const char* str);
void HDSP_printAt(const char* str, size_t pos, size_t count);


#endif // _HDSP_21XX_H_
