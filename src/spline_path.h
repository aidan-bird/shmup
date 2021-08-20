#ifndef SHMUP_SPLINE_PATH_H
#define SHMUP_SPLINE_PATH_H

#include <stdint.h>

#include "./entity.h"

typedef struct PathManager PathManager;
typedef struct PathDef PathDef;
typedef struct PathPoint PathPoint;

typedef struct SplinePath SplinePath;

struct PathPoint
{
    int16_t x;
    int16_t y;
};

struct PathDef
{
    size_t count;
    PathPoint *points;
};

struct SplinePath
{
    size_t count;
    float *coeffA;
    float *coeffB;
    float *coeffC;
    float *coeffD;
};

struct PathManager
{
    EntityPool pool;
};

#endif
