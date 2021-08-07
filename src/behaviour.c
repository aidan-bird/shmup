#include <stdlib.h>

#include "./behaviour.h"
#include "./entity.h"
#include "./config.h"
#include "./content.h"

typedef void (*BehaviourUpdateFunc)(EntityBehaviourManager *mgr,
    unsigned short entityKey);

static void debugbotBehaviourStart(EntityBehaviourManager *mgr,
    unsigned short entityKey);
static void debugbotBehaviourLoop(EntityBehaviourManager *mgr,
    unsigned short entityKey);
static void despawnSelfStart(EntityBehaviourManager *mgr,
    unsigned short entityKey);

/* 
 * XXX there could be multiple behaviour c files, each corresponding to an 
 * entity type
 * to get this to work, expose each behaviour func in .h files, include those 
 * files here, then enter the func names here. update the behaviour func keys
 * accordingly.
 */
static BehaviourUpdateFunc behaviourTab[] = {
    [debugbotStart] = debugbotBehaviourStart,
    [debugbotLoop] = debugbotBehaviourLoop,
    [despawnSelf] = despawnSelfStart,
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
newEntityBehaviourManager(EntityPool *pool, SubsystemsList *subsystems)
{
    size_t n;
    size_t vectorSize;
    EntityBehaviourManager *ret;

    n = pool->count;
    vectorSize = sizeof(unsigned char) + sizeof(unsigned short) 
        + sizeof(unsigned) + sizeof(BehaviourState);
    ret = malloc(sizeof(EntityBehaviourManager) + n * vectorSize);
    if (!ret)
        goto error1;
    ret->poolRef = getEntityPoolRef(pool);
    /* XXX void pointer arithmetic is not portable */
    ret->behaviourKey = (void *)ret + sizeof(EntityBehaviourManager);
    ret->ticksAlive = (void *)ret->behaviourKey + n * sizeof(unsigned char);
    ret->state = (void *)ret->ticksAlive + n * sizeof(unsigned short);
    ret->subsystems = *subsystems;
    /* subscribe to onSpawnEntity Event*/
    if (subscribeToEventManager(pool->onSpawnEntityEvent, ret,
        onSpawnEvent_EntityBehaviourManager,
        "onSpawnEvent_EntityBehaviourManager"))
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
 * releases the memory used by mgr
 */
void
deleteEntityBehaviourManager(EntityBehaviourManager *mgr)
{
    free(mgr);
}

static void
debugbotBehaviourStart(EntityBehaviourManager *mgr, unsigned short entityKey)
{
    /* setup behaviour state */
    mgr->state[entityKey].debugbot.i = 0;
    /* set position */
    mgr->poolRef.x[entityKey] = WIDTH / 2;
    mgr->poolRef.y[entityKey] = HEIGHT / 2;
    /* set collider */
    mgr->subsystems.debug.collider->radius[entityKey] = 32;
    /* set graphics/sprite */
    setAnimation(mgr->subsystems.debug.animator, entityKey, debuganim, 0, 0);
    /* TODO set events e.g., onCollideEvent */
    /* start looping behaviour */
    mgr->behaviourKey[entityKey] = debugbotLoop;
}

/* XXX sample code that demonstrates the behaviour system */
/* XXX used for testing */
static void
debugbotBehaviourLoop(EntityBehaviourManager *mgr, unsigned short entityKey)
{
    mgr->state[entityKey].debugbot.i++;
    mgr->poolRef.x[entityKey]++;
    //mgr->poolRef.y[entityKey] += mgr->state[entityKey].debugbot.i;
    if (mgr->ticksAlive[entityKey] > 120)
        mgr->behaviourKey[entityKey] = despawnSelf;
}

/* XXX used for testing */
static void
despawnSelfStart(EntityBehaviourManager *mgr, unsigned short entityKey)
{
    despawnEntity(mgr->poolRef.pool, entityKey);
}

/* XXX used for testing */
void
onCollisionTestEvent_EntityBehaviourManager(void *nullptr,
    EntityBehaviourManager *mgr, const unsigned short *key)
{
    mgr->behaviourKey[*key] = despawnSelf;
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
        mgr->ticksAlive[j]++;
        if (mgr->behaviourKey[j] == no_behaviour)
            continue;
        behaviourTab[mgr->behaviourKey[j]](mgr, j);
    }
}

