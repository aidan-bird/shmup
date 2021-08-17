#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "./circle_collision.h"
#include "./entity.h"
#include "./utils.h"
#include "./sdlutils.h"
#include "./debug.h"

/*
 * TODO
 * 
 * AABB VS AABB
 * circle vs AABB?
 * point vs point?
 * spatial partitioning?
 * Simd-ify the collision detection algorithms?
 *
 * Make colliders that only test against one entity e.g., the player vs enemy 
 * bullets. The motovaiton for doing this is that for circle vs circle
 * collision detection, the (rp - rx)^2 only needs to be computed once.
 *
 * make a collider that has only one constant radius/bounds; use this for 
 * bullet pools that have only one type of bullet
 */

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * creates a new circle collider from an entity pool. 
 * the collider should not outlive the pool.
 * returns NULL on error.
 */
CircleCollider *
newCircCollider(EntityPool *pool)
{
    size_t vectorSize;
    CircleCollider *ret;

    vectorSize = sizeof(CircleCollider) + sizeof(uint8_t) * pool->count;
    ret = malloc(vectorSize);
    if (!ret)
        return NULL;
    ret->pool = pool;
    ret->radius = (uint8_t *)ret + sizeof(CircleCollider);
    spawnedObjectsCount++;
    return ret;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * collider
 *
 * EFFECTS
 * deletes a collider
 *
 */
void
deleteCircCollider(CircleCollider *collider)
{
    if (!collider)
        return;
    free(collider);
    despawnedObjectsCount++;
}

/* 
 * TODO
 * make a collider struct that only checks agains one entity (the player).
 * this struct will cache radius^2 computations
 */

/*
 * REQUIRES
 * collider.activeIndexMap is updated
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * tests if any circle-bounded entity has collided with a test circle at (x,y) 
 * with radius r. 
 * returns the key of the first entity found to have collided with the test 
 * circle.
 * returns a negative number if no collisions occurred.
 */
uint32_t
testCircCollider(float x, float y, float r, const CircleCollider *collider)
{
    size_t j;
    float diffX;
    float diffY;
    float totalRadius;
    EntityPoolIndexList list;

    list = getEntityPoolIsInitializedIndexList(collider->pool);
    for (size_t i = 0; i < list.n; i++) {
        /* (x2 - x1)^2 + (y2 - y1)^2 <= (r1 + r2)^2 ---> HIT */
        j = list.keys[i];
        diffX = SQUARE(x - collider->pool->x[j]);
        diffY = SQUARE(y - collider->pool->y[j]);
        totalRadius = SQUARE(r + collider->radius[j]);
        if (diffX + diffY <= totalRadius) {
            puts("circle collision detected!");
            return j;
        }
    }
    return -1;
}

void
drawCircCollider(SDL_Renderer *renderer, CircleCollider *collider)
{
    size_t j;
    EntityPoolIndexList list;

    list = getEntityPoolIsInitializedIndexList(collider->pool);
    for (size_t i = 0; i < list.n; i++) {
        j = list.keys[i];
        drawCircle(renderer, collider->pool->x[j], collider->pool->y[j],
            collider->radius[j]);
    }
}

