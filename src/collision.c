#include <stdlib.h>
#include <stdio.h>

#include "./collision.h"
#include "./entity.h"
#include "./utils.h"

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * creates a new circle collider from a entity pool. 
 * the pool should have the same lifetime as the circle collider since it 
 * references data stored in the pool.
 */
CircleCollider *
newCircCollider(const EntityPool *pool)
{
    CircleCollider *ret;
    size_t n;

    n = pool->count;
    ret = malloc(sizeof(CircleCollider) + sizeof(unsigned char) * n);
    if (!ret)
        return NULL;
    ret->count = n;
    ret->activeCount = &pool->activeCount;
    ret->isActive = pool->isActive;
    ret->activeIndexMap = pool->activeIndexMap;
    ret->radius = (void *)ret + sizeof(unsigned char) * n;
    ret->x = pool->x;
    ret->y = pool->y;
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
    for (int i = 0; i < *collider->activeCount; i++) {
        /* (x2 - x1)^2 + (y2 - y1)^2 <= (r1 + r2)^2 ---> HIT */
        if (SQUARE(x - collider->x[collider->activeIndexMap[i]]) + SQUARE(y - 
            collider->y[collider->activeIndexMap[i]]) <= SQUARE(r + 
            collider->radius[collider->activeIndexMap[i]])) {
            puts("circle collision detected!");
            return collider->activeIndexMap[i];
        }
    }
    return -1;
}


