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
 * pool is a valid pointer to a uninitialized pool
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * initializes pool; this must be called before pool is used.
 * returns non-zero on error;
 */
int
initEntityPool(EntityPool *pool)
{
    if (newEventManager(&pool->onSpawnEntityEvent, pool, "On Spawn Entity"))
        return -1;
    return 0;
}

void
invalidateEntityPool(EntityPool *pool)
{
    /* TODO associated subsystems could be freed here */
    deleteEventManager(pool->onSpawnEntityEvent);
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
    if (pool->next + 1 < pool->count) {
        pool->next++;
        pool->activeCount++;
    } else {
        pool->next = 0;
    }
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
