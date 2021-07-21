#include "./entity.h"
#include "./config.h"

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
 *
 * MODIFIES
 *
 * EFFECTS
 *
 */
void
drawEntityPool(SDL_Renderer *renderer, const EntityPool *pool)
{
}

