#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#include "./assets.h"

#define POOL_FLAGS_AABB (1 << 0)
#define POOL_FLAGS_CIRC (1 << 1)
#define POOL_FLAGS_NO_COLLISION (1 << 2)
#define POOL_FLAGS_NO_ON_COLLISION_RECORD (1 << 3)

#define TEST_POOL_FLAGS_AABB(POOL_PTR) ((POOL_PTR)->poolFlags & (POOL_FLAGS_AABB) == 1)
#define TEST_POOL_FLAGS_CIRC(POOL_PTR) ((POOL_PTR)->poolFlags & (POOL_FLAGS_CIRC) == 1)
#define TEST_POOL_FLAGS_NO_COLLISION(POOL_PTR) ((POOL_PTR)->poolFlags & (POOL_FLAGS_NO_COLLISION) == 1)
#define TEST_POOL_FLAGS_NO_ON_COLLISION_RECORD(POOL_PTR) ((POOL_PTR)->poolFlags & (POOL_FLAGS_NO_ON_COLLISION_RECORD) == 1)

/* use this to define a new EntityPool */
#define newDebugEntityPool(SIZE) \
(EntityPool) { \
    .next = 0, \
    .activeCount = 0, \
    .count = (SIZE), \
    .poolFlags = ((POOL_FLAGS_AABB) | (POOL_FLAGS_CIRC) \
        | (POOL_FLAGS_NO_ON_COLLISION_RECORD)), \
    .isActive = (char[(SIZE)]){}, \
    .spriteID = (unsigned short[(SIZE)]){}, \
    .behaviourID = (unsigned short[(SIZE)]){}, \
    .aabbWidth = (unsigned short[(SIZE)]){}, \
    .aabbHeight = (unsigned short[(SIZE)]){}, \
    .circRad = (unsigned short[(SIZE)]){}, \
    .activeIndexMap = (unsigned short[(SIZE)]){}, \
    .ticksAlive = (unsigned[(SIZE)]){}, \
    .flags = (unsigned[(SIZE)]){}, \
    .x = (float[(SIZE)]){}, \
    .y = (float[(SIZE)]){}, \
    .rot = (float[(SIZE)]){}, \
    .vel = (float[(SIZE)]){}, \
    .accel = (float[(SIZE)]){}, \
};

typedef struct EntityPool EntityPool;
typedef struct EntityPoolRef EntityPoolRef;

void drawEntityPool(SDL_Renderer *renderer, const EntityPool *pool,
    const AssetTable *assets);
void updateActiveIndexMap(EntityPool *pool);
void updateEntityPool(EntityPool *pool);
void updateEntityPosition(EntityPool *pool);
void despawnEntity(EntityPool *pool, unsigned short key);
unsigned short spawnEntity(EntityPool *pool);

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
    unsigned poolFlags;
    char *isActive;

    /* XXX deprecated, components and subsystems will be used instead */
    unsigned short *behaviourID;
    unsigned short *spriteID;
    unsigned short *aabbWidth;
    unsigned short *aabbHeight;
    unsigned short *circRad;

    unsigned short *activeIndexMap;
    unsigned *ticksAlive;
    unsigned *flags;
    float *x;
    float *y;
    float *rot;
    float *vel;
    float *accel;
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
    const float *x;
    const float *y;
};

EntityPoolRef getEntityPoolRef(const EntityPool *pool);

extern EntityPool playerBullets;
extern EntityPool enemyBullets;
extern EntityPool enemies;
extern EntityPool pickups;

#endif
