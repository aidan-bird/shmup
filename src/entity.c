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
    
    vectorSize = sizeof(EntityPool) + n * (2 * sizeof(uint8_t) 
        + 2 * sizeof(uint16_t) + sizeof(uint32_t) + 2 * sizeof(float));
    ret = malloc(vectorSize);
    if (!ret)
        goto error1;
    ret->next = 0;
    ret->activeCount = 0;
    ret->count = n;
    ret->isActive = (uint8_t *)ret + sizeof(EntityPool);
    ret->activeIndexMap = (uint16_t *)((uint8_t *)ret->isActive
        + n * sizeof(uint8_t));
    ret->flags = (uint32_t *)((uint8_t *)ret->activeIndexMap
        + n * sizeof(uint16_t));
    ret->x = (float *)((uint8_t *)ret->flags + n * sizeof(uint32_t));
    ret->y = (float *)((uint8_t *)ret->x + n * sizeof(float));
    ret->isInitialized = (uint8_t *)((uint8_t *)ret->y + n * sizeof(float));
    ret->isInitializedIndexMap = (uint16_t *)((uint8_t *)ret->isInitialized
        + n * sizeof(uint8_t));
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
uint16_t
spawnEntity(EntityPool *pool)
{
    uint16_t ret;

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
despawnEntity(EntityPool *pool, uint16_t key)
{
    if (pool->isActive[key]) {
        pool->isActiveIndexMapDirty = 1;
        pool->isActive[key] = 0;
        pool->activeCount--;
    }
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

