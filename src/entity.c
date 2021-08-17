#include <SDL.h>

#include "./entity.h"
#include "./config.h"
#include "./assets.h"
#include "./content.h"

EntityPool playerBullets = newDebugEntityPool(POOL_SIZE);
EntityPool enemyBullets = newDebugEntityPool(POOL_SIZE);
EntityPool enemies = newDebugEntityPool(POOL_SIZE);
EntityPool pickups = newDebugEntityPool(POOL_SIZE);

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * creates a new pool on the heap.
 * returns NULL on error.
 */
EntityPool *
newEntityPool(size_t n)
{
    size_t vectorSize;
    EntityPool *ret;
    
    vectorSize = sizeof(EntityPool) + n * (2 * sizeof(char) 
        + 2 * sizeof(unsigned short) + sizeof(unsigned) + 2 * sizeof(float));
    ret = malloc(vectorSize);
    if (!ret)
        goto error1;
    ret->next = 0;
    ret->activeCount = 0;
    ret->count = n;
    ret->isActive = (void *)ret + sizeof(EntityPool);
    ret->activeIndexMap = (void *)ret->isActive + n * sizeof(char);
    ret->flags = (void *)ret->activeIndexMap + n * sizeof(unsigned short);
    ret->x = (void *)ret->flags + n * sizeof(unsigned);
    ret->y = (void *)ret->x + n * sizeof(float);
    ret->isInitialized = (void *)ret->y + n * sizeof(float);
    ret->isInitializedIndexMap = (void *)ret->isInitialized + n * sizeof(char);
    ret->onSpawnEntityEvent = NULL;
    return ret;
error1:;
    return NULL;
}

/*
 * REQUIRES
 * pool was created using newEntityPool()
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * deletes pool and any associated event managers if they were registered
 */
void
deleteEntityPool(EntityPool *pool)
{
    if (!pool)
        return;
    if (pool->onSpawnEntityEvent)
        deleteEventManager(pool->onSpawnEntityEvent);
    free(pool);
}

/*
 * REQUIRES
 * pool is valid
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * sets up a Event Manager for notifying subscribers when an entity is spawned.
 * returns non-zero on error.
 */
int
addOnSpawnEntityEventManager(EntityPool *pool)
{
    if (!pool->onSpawnEntityEvent)  {
        if (!(pool->onSpawnEntityEvent = newEventManager(pool, 
            "OnSpawnEntity Event")))
            return -1;
    }
    return 0;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * creates a new EntityPoolRef which is associated with pool.
 */
EntityPoolRef
getEntityPoolRef(const EntityPool *pool)
{
    return (EntityPoolRef) {
        .count = pool->count, 
        .activeCount = &pool->activeCount,
        .isActive = pool->isActive,
        .activeIndexMap = pool->activeIndexMap,
        .x = pool->x,
        .y = pool->y,
        .pool = pool,
    };
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * updates pool.activeIndexMap to contain the indexes of the active entities.  
 */
void
updateActiveIndexMap(EntityPool *pool)
{
    int i;
    int j;
    int k;

    if (!pool->isActiveIndexMapDirty)
        return;
    pool->isActiveIndexMapDirty = 0;
    i = pool->activeCount;
    j = 0;
    k = 0;
    while (i) {
        if (pool->isActive[j]) {
            pool->activeIndexMap[k] = j;
            i--;
            k++;
        }
        j++;
    }
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * marks an entity as spawned, returns this entity's key.
 * If pool is saturated then the key of an active entity may be returned. 
 */
unsigned short
spawnEntity(EntityPool *pool)
{
    unsigned short ret;

    ret = pool->next;
    pool->isActive[ret] = 1;
    pool->isInitialized[ret] = 0;
    pool->isInitializedIndexMapDirty = 1;
    pool->isActiveIndexMapDirty = 1;
    if (pool->next + 1 < pool->count) {
        pool->next++;
        pool->activeCount++;
    } else {
        pool->next = 0;
    }
    if (pool->onSpawnEntityEvent)
        raiseEvent(pool->onSpawnEntityEvent, &ret);
    return ret;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * marks the entity identified by key as despawned.
 */
void
despawnEntity(EntityPool *pool, unsigned short key)
{
    if (pool->isActive[key]) {
        pool->isActiveIndexMapDirty = 1;
        pool->isActive[key] = 0;
        pool->activeCount--;
    }
}

/*
 * REQUIRES
 *
 * MODIFIES
 *
 * EFFECTS
 *
 */
void
updateEntityPosition(EntityPool *pool)
{

}

/*
 * REQUIRES
 *
 * MODIFIES
 *
 * EFFECTS
 *
 */
void
updateEntityPool(EntityPool *pool)
{
    updateActiveIndexMap(pool);
    /* update position */

    /* do collision detection */

    /* set events flags */

    /* update behavior */
}

EntityPoolIndexList
getEntityPoolActiveIndexList(EntityPool *pool)
{
    updateActiveIndexMap(pool);
    return (EntityPoolIndexList) {
        .n = pool->activeCount,
        .keys = pool->activeIndexMap,
    };
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * calls updateActiveIndexMap(), and then updates the isInitializedIndexMap
 */
size_t
updateIsInitializedIndexMap(EntityPool *pool)
{
    size_t j;
    size_t k;

    if (!pool->isInitializedIndexMapDirty)
        return pool->isInitializedCount;
    updateActiveIndexMap(pool);
    k = 0;
    for (size_t i = 0; i < pool->activeCount; i++) {
        j = pool->activeIndexMap[i];
        if (pool->isInitialized[j]) {
            pool->isInitializedIndexMap[k] = j;
            k++;
        }
    }
    pool->isInitializedCount = k;
    pool->isInitializedIndexMapDirty = 0;
    return k;
}

EntityPoolIndexList
getEntityPoolIsInitializedIndexList(EntityPool *pool)
{
    size_t n;

    n = updateIsInitializedIndexMap(pool);
    return (EntityPoolIndexList) {
        .n = n,
        .keys = pool->isInitializedIndexMap,
    };
}

