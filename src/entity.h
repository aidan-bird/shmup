#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#include "./assets.h"
#include "./event.h"

#define raiseIsInitializedFlag(POOL_PTR, KEY) \
    ((EntityPool*)POOL_PTR)->isInitialized[(KEY)] = 1; \
    ((EntityPool*)POOL_PTR)->isInitializedIndexMapDirty = 1;

/*
 * TODO
 *
 * create a bullet pool with only one type of entity.
 */

/*
 * IDEA
 *
 * let x,y of all pools refer to a contiguous global array.
 */

/* use this to define a new EntityPool */
#define newDebugEntityPool(SIZE) \
(EntityPool) { \
    .next = 0, \
    .activeCount = 0, \
    .isInitializedIndexMapDirty = 0, \
    .isInitializedCount = 0, \
    .count = (SIZE), \
    .isActive = (char[(SIZE)]){}, \
    .activeIndexMap = (unsigned short[(SIZE)]){}, \
    .flags = (unsigned[(SIZE)]){}, \
    .x = (float[(SIZE)]){}, \
    .y = (float[(SIZE)]){}, \
    .isInitialized = (char[(SIZE)]){}, \
    .isInitializedIndexMap = (unsigned short[(SIZE)]){}, \
    .onSpawnEntityEvent = NULL, \
};

typedef struct EntityPool EntityPool;
typedef struct EntityPoolRef EntityPoolRef;
typedef struct EntityPoolIndexList EntityPoolIndexList;

struct EntityPoolIndexList
{
    size_t n;
    const unsigned short *keys;
};

int addOnSpawnEntityEventManager(EntityPool *pool);
void updateActiveIndexMap(EntityPool *pool);
void updateEntityPool(EntityPool *pool);
void updateEntityPosition(EntityPool *pool);
void despawnEntity(EntityPool *pool, unsigned short key);
void invalidateEntityPool(EntityPool *pool);
unsigned short spawnEntity(EntityPool *pool);
EntityPoolIndexList getEntityPoolActiveIndexList(EntityPool *pool);
EntityPool *newEntityPool(size_t n);
size_t updateIsInitializedIndexMap(EntityPool *pool);
EntityPoolIndexList getEntityPoolIsInitializedIndexList(EntityPool *pool);

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
    char isInitializedIndexMapDirty;
    size_t isInitializedCount;
    char *isActive;
    unsigned short *activeIndexMap;
    unsigned *flags;
    float *x;
    float *y;
    EventManager *onSpawnEntityEvent;
    char isActiveIndexMapDirty;
    char *isInitialized;
    unsigned short *isInitializedIndexMap;
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
    const EntityPool *pool;
};

EntityPoolRef getEntityPoolRef(const EntityPool *pool);

extern EntityPool playerBullets;
extern EntityPool enemyBullets;
extern EntityPool enemies;
extern EntityPool pickups;

#endif
