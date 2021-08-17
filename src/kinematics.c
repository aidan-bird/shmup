#include <stdlib.h>
#include <math.h>

#include "./kinematics.h"
#include "./utils.h"
#include "./event.h"
#include "./debug.h"

static void
onSpawnEvent_KinematicsManager(const EntityPool *caller,
    KinematicsManager *subscriber, const uint16_t *args);

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * returns a new Kinematics Manager.
 * returns NULL on error.
 */
KinematicsManager *
newKinematicsManager(EntityPool *pool)
{
    size_t n;
    size_t vectorSize;
    KinematicsManager *ret;

    n = pool->count;
    vectorSize = sizeof(KinematicsManager) + n * (6 * sizeof(float)
        + sizeof(uint8_t));
    ret = malloc(vectorSize);
    if (!ret)
        goto error1;
    ret->pool = pool;
    ret->speed = (float *)((uint8_t *)ret + sizeof(KinematicsManager));
    ret->rot = (float *)((uint8_t *) ret->speed + n * sizeof(float));
    ret->accel = (float *)((uint8_t *) ret->rot + n * sizeof(float));
    ret->angularVelocity = (float *)((uint8_t *) ret->accel + n * sizeof(float));
    ret->rotCacheX = (float *)((uint8_t *) ret->angularVelocity + n * sizeof(float));
    ret->rotCacheY = (float *)((uint8_t *) ret->rotCacheX + n * sizeof(float));
    ret->isUpdateRot = (uint8_t *)ret->rotCacheY + n * sizeof(float);
    if (subscribeToEventManager(pool->onSpawnEntityEvent, ret,
        (OnEventFunc)onSpawnEvent_KinematicsManager,
        "onSpawnEvent_KinematicsManager"))
        goto error2;
    spawnedObjectsCount++;
    return ret;
error2:;
    free(ret);
error1:;
    return NULL;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 * updates mgr
 */
void
updateKinematicsManager(KinematicsManager *mgr)
{
    size_t j;
    EntityPoolIndexList list;

    list = getEntityPoolIsInitializedIndexList(mgr->pool);
    for (size_t i = 0; i < list.n; i++) {
        j = list.keys[i];
        mgr->speed[j] += mgr->accel[j];
        if (mgr->angularVelocity[j]) {
            mgr->rot[j] += mgr->angularVelocity[j];
            mgr->isUpdateRot[j] = 1;
        }
        /* TODO test if the caching is actually faster */
        if (mgr->isUpdateRot[j]) {
            mgr->rotCacheX[j] = cosf(mgr->rot[j]);
            mgr->rotCacheY[j] = sinf(mgr->rot[j]);
            mgr->isUpdateRot[j] = 0;
        }
        mgr->pool->x[j] += mgr->speed[j] * mgr->rotCacheX[j];
        mgr->pool->y[j] += mgr->speed[j] * mgr->rotCacheY[j];
    }
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 * deletes mgr
 */
void
deleteKinematicsManager(KinematicsManager *mgr)
{
    if (!mgr)
        return;
    despawnedObjectsCount++;
    free(mgr);
}

static void
onSpawnEvent_KinematicsManager(const EntityPool *caller,
    KinematicsManager *subscriber, const uint16_t *args)
{
    subscriber->isUpdateRot[*args] = 1;
}
