/*!

\author         Oliver Blaser
\date           19.04.2021
\copyright      GNU GPLv3 - Copyright (c) 2021 Oliver Blaser

*/

#ifndef _PROJECT_H_
#define	_PROJECT_H_



#define PRJ_DEBUG (1)

#define PRJ_PROGIO_MIN (1)
#define PRJ_PROGIO_MOUT (2)
#define PRJ_PROGIO_MCLKOUT (3)

#define PRJ_PROGIO_MODE PRJ_PROGIO_MIN

#define PRJ_PROGIO_IN (PRJ_PROGIO_MODE == PRJ_PROGIO_MIN)
#define PRJ_PROGIO_OUT (PRJ_PROGIO_MODE == PRJ_PROGIO_MOUT)
#define PRJ_PROGIO_CLKOUT (PRJ_PROGIO_MODE == PRJ_PROGIO_MCLKOUT)



#endif // _PROJECT_H_
