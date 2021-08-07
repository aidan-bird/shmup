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

/*
 * REQUIRES
 * pool is valid
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * invalidates pool.
 */
void
invalidateEntityPool(EntityPool *pool)
{
    /* TODO associated subsystems could be freed here */
    deleteEventManager(pool->onSpawnEntityEvent);
}

EntityPoolActiveIndexList
getEntityPoolActiveIndexList(EntityPool *pool)
{
    updateActiveIndexMap(pool);
    return (EntityPoolActiveIndexList) {
        .n = pool->activeCount,
        .keys = pool->activeIndexMap,
    };
}

