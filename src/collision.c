#include <stdlib.h>
#include <stdio.h>

#include "./collision.h"
#include "./entity.h"
#include "./utils.h"

/*
 * TODO
 * 
 * AABB VS AABB
 * circle vs AABB?
 * point vs point?
 * Simd-ify the collision detection algorithms?
 *
 * Make colliders that only test against one entity e.g., the player vs enemy 
 * bullets. The motovaiton for doing this is that for circle vs circle
 * collision detection, the (rp - rx)^2 only needs to be computed once.
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
newCircCollider(const EntityPool *pool)
{
    CircleCollider *ret;

    ret = malloc(sizeof(CircleCollider) + sizeof(unsigned char) * pool->count);
    if (!ret)
        return NULL;
    ret->poolRef = getEntityPoolRef(pool);
    ret->radius = (void *)ret + sizeof(CircleCollider);
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
    free(collider);
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
short
testCircCollider(float x, float y, float r, const CircleCollider *collider)
{
    size_t j;
    float diffX;
    float diffY;
    float totalRadius;

    for (int i = 0; i < *collider->poolRef.activeCount; i++) {
        /* (x2 - x1)^2 + (y2 - y1)^2 <= (r1 + r2)^2 ---> HIT */
        j = collider->poolRef.activeIndexMap[i];
        diffX = SQUARE(x - collider->poolRef.x[j]);
        diffY = SQUARE(y - collider->poolRef.y[j]);
        totalRadius = SQUARE(r + collider->radius[j]);
        if (diffX + diffY <= totalRadius) {
            puts("circle collision detected!");
            return j;
        }
    }
    return -1;
}


