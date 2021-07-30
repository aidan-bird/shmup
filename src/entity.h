#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#include "./assets.h"
#include "./event.h"

/* use this to define a new EntityPool */
#define newDebugEntityPool(SIZE) \
(EntityPool) { \
    .next = 0, \
    .activeCount = 0, \
    .count = (SIZE), \
    .isActive = (char[(SIZE)]){}, \
    .activeIndexMap = (unsigned short[(SIZE)]){}, \
    .flags = (unsigned[(SIZE)]){}, \
    .x = (float[(SIZE)]){}, \
    .y = (float[(SIZE)]){}, \
};

typedef struct EntityPool EntityPool;
typedef struct EntityPoolRef EntityPoolRef;

void updateActiveIndexMap(EntityPool *pool);
void updateEntityPool(EntityPool *pool);
void updateEntityPosition(EntityPool *pool);
void despawnEntity(EntityPool *pool, unsigned short key);
unsigned short spawnEntity(EntityPool *pool);
int initEntityPool(EntityPool *pool);
void invalidateEntityPool(EntityPool *pool);

/*
 * Represents a fixed-length pool of objects.
 * Newly created objects will be stored in this pool instead of allocating 
 * more memory on the heap.
 * If too many objects are spawned, the memory of objects that are already in 
 * use may be used for new objects.
 */
struct EntityPool
{
    size_t next;
    size_t activeCount;
    size_t count;
    char *isActive;
    unsigned short *activeIndexMap;
    unsigned *flags;
    float *x;
    float *y;
    EventManager *onSpawnEntityEvent;
};

/* 
 * Allows subsystems to be associated with an entity pool instance.
 * This struct provides these subsystems with limited access to some fields 
 * of the associated entity pool. This struct should not outlive the associated 
 * entity pool.
 */
struct EntityPoolRef
{
    size_t count;
    const size_t *activeCount;
    const char *isActive;
    const unsigned short *activeIndexMap;
    float *x;
    float *y;
};

EntityPoolRef getEntityPoolRef(const EntityPool *pool);

extern EntityPool playerBullets;
extern EntityPool enemyBullets;
extern EntityPool enemies;
extern EntityPool pickups;

#endif
