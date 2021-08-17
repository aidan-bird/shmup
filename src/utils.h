#ifndef SHMUP_UTILS_H
#define SHMUP_UTILS_H

#define ROOT1_2F ((float)0.707106781186547)
#define PI_F ((float)3.141592653589793)

#define LEN(X) (sizeof(X) / sizeof((X)[0]))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define SQUARE(X) ((X) * (X))
#define DEG2RAD_F(X) ((X) * PI_F / 180)

#endif
