#include <stdlib.h>

#include "./behaviour.h"
#include "./entity.h"

typedef void (*BehaviourUpdateFunc)(EntityBehaviourManager *mgr,
    unsigned short entityKey);

static void debugbotBehaviour(EntityBehaviourManager *mgr,
    unsigned short entityKey);

static BehaviourUpdateFunc behaviourTab[] = {
    [debugbot] = debugbotBehaviour,
};

/*
 * REQUIRES
 * all pointer arguments are valid.
 * args is a pointer to an Entity Key.
 *
 * MODIFIES
 * subscriber
 *
 * EFFECTS
 * sets up the behaviour entry of an entity when it is spawned.
 */
void
onSpawnEvent_EntityBehaviourManager(EntityPool *caller, void *subscriber,
    const void *args)
{
    EntityBehaviourManager *mgr;
    unsigned short entityKey;

    mgr = subscriber;
    entityKey = *(unsigned short *)args;
    mgr->ticksAlive[entityKey] = 0;
    mgr->flags[entityKey] = 0;
}

/*
 * REQUIRES
 * pool is a valid pointer to an Entity Pool.
 *
 * MODIFIES
 * pool
 *
 * EFFECTS
 * Creates and assigns a new Behaviour Manager to a pool.
 * The Behaviour Manager must not outlive the pool.
 */
EntityBehaviourManager *
newEntityBehaviourManager(EntityPool *pool)
{
    size_t n;
    size_t vectorSize;
    EntityBehaviourManager *ret;

    n = pool->count;
    vectorSize = sizeof(unsigned char) + sizeof(unsigned short) 
        + sizeof(unsigned);
    ret = malloc(sizeof(EntityBehaviourManager) + n * vectorSize);
    if (!ret)
        goto error1;
    ret->poolRef = getEntityPoolRef(pool);
    ret->behaviourKey = (void *)ret + sizeof(EntityBehaviourManager);
    ret->ticksAlive = (void *)ret->behaviourKey + n * sizeof(unsigned char);
    ret->flags = (void *)ret->ticksAlive + n * sizeof(unsigned short);
    if (subscribeToEventManager(pool->onSpawnEntityEvent, ret,
        onSpawnEvent_EntityBehaviourManager,
        "onSpawnEvent_EntityBehaviourManager"))
        goto error2;
    return ret;
error3:;

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
 * releases the memory used by mgr
 */
void
deleteEntityBehaviourManager(EntityBehaviourManager *mgr)
{
    free(mgr);
}

/*
 * REQUIRES
 * mgr is a pointer to a valid Behaviour Manager.
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 *
 */
static void
debugbotBehaviour(EntityBehaviourManager *mgr, unsigned short entityKey)
{
    mgr->poolRef.x[entityKey]++;
}

/*
 * REQUIRES
 * mgr is a pointer to a valid Behaviour Manager.
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 * updates mgr
 */
void
updateEntityBehaviourManager(EntityBehaviourManager *mgr)
{
    size_t j;

    for (int i = 0; i < *mgr->poolRef.activeCount; i++) {
        j = mgr->poolRef.activeIndexMap[i];
        if (mgr->behaviourKey[j] == no_behaviour)
            continue;
        behaviourTab[mgr->behaviourKey[j]](mgr, j);
    }
}

