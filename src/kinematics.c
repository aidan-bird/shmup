#include <stdlib.h>
#include <math.h>

#include "./kinematics.h"
#include "./utils.h"
#include "./event.h"

#define N_FLOATS 6

static void
onSpawnEvent_KinematicsManager(const EntityPool *caller,
    KinematicsManager *subscriber, const unsigned short *args);

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
    vectorSize = N_FLOATS * sizeof(float);
    ret = malloc(sizeof(KinematicsManager) + n * vectorSize);
    if (!ret)
        goto error1;
    ret->poolRef = getEntityPoolRef(pool);
    float **retFloats[] = {
        &ret->speed, &ret->rot, &ret->accel, &ret->angularVelocity,
        &ret->rotCacheX, &ret->rotCacheY,
    };
    *retFloats[0] = (void *)ret + sizeof(KinematicsManager);
    for (int i = 1; i < N_FLOATS; i++)
        *retFloats[i] = (void *)(*(retFloats[i - 1])) + n * sizeof(float);
    ret->isUpdateRot = (void *)(*(retFloats[N_FLOATS - 1])) + n * sizeof(float);
    if (subscribeToEventManager(pool->onSpawnEntityEvent, ret,
        onSpawnEvent_KinematicsManager, "onSpawnEvent_KinematicsManager"))
        goto error2;
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
    EntityPoolActiveIndexList list;

    list = getEntityPoolActiveIndexList(mgr->poolRef.pool);
    for (int i = 0; i < list.n; i++) {
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
        mgr->poolRef.pool->x[j] += mgr->speed[j] * mgr->rotCacheX[j];
        mgr->poolRef.pool->y[j] += mgr->speed[j] * mgr->rotCacheY[j];
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
    free(mgr);
}

static void
onSpawnEvent_KinematicsManager(const EntityPool *caller,
    KinematicsManager *subscriber, const unsigned short *args)
{
    subscriber->isUpdateRot[*args] = 1;
}
