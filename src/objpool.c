#include "./objpool.h"

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * returns a pointer to a newly created object pool.
 * returns NULL on error.
 */
ObjPool *
newObjPool(size_t n)
{
    ObjPool *ret;

    if (!(ret = malloc(sizeof(ObjPool) + n * sizeof(char))))
        return NULL;
    ret->active = (void *)ret + sizeof(ObjPool);
    ret->capacity = n;
    ret->count = 0;
    ret->next = 0;
    return ret;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * deletes pool.
 */
void
deleteObjPool(ObjPool *pool)
{
    free(pool);
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * marks the object identified by key as despawned.
 */
void
objPoolDespawn(ObjPool *pool, ushort key)
{
    pool->active[key] = 0;
    pool->count--;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * marks an object as spawned, returns this object's key.
 * If pool is saturated then the key of an active object may be returned.
 */
ushort
objPoolSpawn(ObjPool *pool)
{
    ushort ret;

    if (pool->next >= pool->capacity)
        pool->next = 0;
    ret = pool->next;
    pool->active[ret] = 1;
    pool->next++;
    pool->count++;
    return ret;
}

