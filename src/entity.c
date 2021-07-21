#include <SDL.h>

#include "./entity.h"
#include "./config.h"
#include "./shmup.h"
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

/*
 * REQUIRES
 * pool.activeIndexMap was updated before calling this
 *
 * MODIFIES
 * renderer
 *
 * EFFECTS
 * draws all active entities
 */
void
drawEntityPool(SDL_Renderer *renderer, const EntityPool *pool)
{
    SDL_Rect src;
    SDL_Rect dest;
    const AssetTable *assets;

    assets = getDebugAssetTable();
    if (!assets)
        return;
    for (int i = 0; i < pool->activeCount; i++) {
        src = (SDL_Rect) { 
            .x = 0,
            .y = 0,
            .w = 32,
            .h = 32,
        };
        dest = (SDL_Rect) {
            .x = pool->x[pool->activeIndexMap[i]],
            .y = pool->y[pool->activeIndexMap[i]],
            .w = pool->aabbWidth[pool->activeIndexMap[i]],
            .h = pool->aabbHeight[pool->activeIndexMap[i]],
        };
        SDL_RenderCopy(renderer, assets->assets[debugred], &src, &dest);
    }
}

