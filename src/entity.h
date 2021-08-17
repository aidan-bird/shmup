#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <stdint.h>

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
{ \
    .next = 0, \
    .activeCount = 0, \
    .isInitializedIndexMapDirty = 0, \
    .isInitializedCount = 0, \
    .count = (SIZE), \
    .isActive = (uint8_t[(SIZE)]){0}, \
    .activeIndexMap = (uint16_t[(SIZE)]){0}, \
    .flags = (uint32_t[(SIZE)]){0}, \
    .x = (float[(SIZE)]){0}, \
    .y = (float[(SIZE)]){0}, \
    .isInitialized = (uint8_t[(SIZE)]){0}, \
    .isInitializedIndexMap = (uint16_t[(SIZE)]){0}, \
    .onSpawnEntityEvent = NULL, \
}


typedef struct EntityPool EntityPool;
typedef struct EntityPoolRef EntityPoolRef;
typedef struct EntityPoolIndexList EntityPoolIndexList;

struct EntityPoolIndexList
{
    size_t n;
    const uint16_t *keys;
};

int addOnSpawnEntityEventManager(EntityPool *pool);
void updateActiveIndexMap(EntityPool *pool);
void despawnEntity(EntityPool *pool, uint16_t key);
void invalidateEntityPool(EntityPool *pool);
uint16_t spawnEntity(EntityPool *pool);
EntityPoolIndexList getEntityPoolActiveIndexList(EntityPool *pool);
EntityPool *newEntityPool(size_t n);
size_t updateIsInitializedIndexMap(EntityPool *pool);
EntityPoolIndexList getEntityPoolIsInitializedIndexList(EntityPool *pool);
void deleteEntityPool(EntityPool *pool);

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
    uint8_t isInitializedIndexMapDirty;
    size_t isInitializedCount;
    uint8_t *isActive;
    uint16_t *activeIndexMap;
    uint32_t *flags;
    float *x;
    float *y;
    EventManager *onSpawnEntityEvent;
    uint8_t isActiveIndexMapDirty;
    uint8_t *isInitialized;
    uint16_t *isInitializedIndexMap;
};

extern EntityPool playerBullets;
extern EntityPool enemyBullets;
extern EntityPool enemies;
extern EntityPool pickups;

#endif
