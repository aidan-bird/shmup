#ifndef OBJPOOL_H
#define OBJPOOL_H

#include <stdlib.h>

#include "./utils.h"

typedef struct ObjPool ObjPool;

struct ObjPool
{
    size_t count;
    size_t capacity;
    size_t next;
    char *active;
};

ObjPool * newObjPool(size_t n);
void deleteObjPool(ObjPool *pool);
void objPoolDespawn(ObjPool *pool, ushort key);
ushort objPoolSpawn(ObjPool *pool);

#endif
