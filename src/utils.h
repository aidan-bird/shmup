#ifndef SHMUP_UTILS_H
#define SHMUP_UTILS_H

#include <time.h>
#include <stdio.h>

#define ROOT1_2F ((float)0.707106781186547)
#define PI_F ((float)3.141592653589793)

#define LEN(X) (sizeof(X) / sizeof((X)[0]))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define SQUARE(X) ((X) * (X))
#define DEG2RAD_F(X) ((X) * PI_F / 180)
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

/* 
 * Used for getting the execution time of EXPR
 * The time delta is stored at DOUBLE_PTR
 */
#define getExecTime(EXPR, DOUBLE_PTR) \
{ \
    clock_t start; \
    clock_t end; \
    start = clock(); \
    (EXPR); \
    end = clock(); \
    *(DOUBLE_PTR) = ((double) (end - start)) / CLOCKS_PER_SEC; \
}

#endif
