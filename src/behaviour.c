#include <stdlib.h>

#include "./behaviour.h"
#include "./entity.h"
#include "./config.h"
#include "./content.h"
#include "./kinematics.h"

typedef void (*BehaviourUpdateFunc)(EntityBehaviourManager *mgr,
    unsigned short entityKey);

static void debugbotBehaviourStart(EntityBehaviourManager *mgr,
    unsigned short entityKey);
static void debugbotBehaviourLoop(EntityBehaviourManager *mgr,
    unsigned short entityKey);
static void despawnSelfStart(EntityBehaviourManager *mgr,
    unsigned short entityKey);
static void simpleBulletBehaviourStart(EntityBehaviourManager *mgr,
    unsigned short entityKey);

EntityBehaviourManager *newEntityBehaviourManager(EntityPool *pool,
    SubsystemsList *subsystems);

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
    [simpleBulletStart] = simpleBulletBehaviourStart,
};

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 * sets the behaviour of an entity.
 * args is optional
 */
void
setBehaviour(EntityBehaviourManager *mgr, const BehaviourArgs *args, 
    BehaviourKey behaviourKey, unsigned short entityKey)
{
    mgr->behaviourKey[entityKey] = behaviourKey;
    mgr->ticksDelta[entityKey] = 0;
    if (args)
        mgr->args[entityKey] = *args;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * pool, animator, collider
 *
 * EFFECTS
 * creates a new Entity Behaviour Manager for debugging/testing
 */
EntityBehaviourManager *
newDebugEntityBehaviourManager(EntityPool *pool, Animator *animator,
    CircleCollider *collider, KinematicsManager *kinematics)
{
    SubsystemsList subsys;

    subsys = (SubsystemsList) {
        .debug = {
            .animator = animator,
            .collider = collider,
            .kinematics = kinematics,
        },
    };
    return newEntityBehaviourManager(pool, &subsys);
}

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
onSpawnEvent_EntityBehaviourManager(EntityPool *caller, 
    EntityBehaviourManager *subscriber, const unsigned short *args)
{
    subscriber->ticksAlive[*args] = 0;
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
    vectorSize = sizeof(unsigned char) + 2 * sizeof(unsigned short) 
        + sizeof(unsigned) + sizeof(BehaviourState) + sizeof(BehaviourArgs);
    ret = malloc(sizeof(EntityBehaviourManager) + n * vectorSize);
    if (!ret)
        goto error1;
    ret->poolRef = getEntityPoolRef(pool);
    /* XXX void pointer arithmetic is not portable */
    ret->behaviourKey = (void *)ret + sizeof(EntityBehaviourManager);
    ret->ticksAlive = (void *)ret->behaviourKey + n * sizeof(unsigned char);
    ret->state = (void *)ret->ticksAlive + n * sizeof(unsigned short);
    ret->args = (void *)ret->state + n * sizeof(BehaviourState);
    ret->ticksDelta = (void *)ret->args + n * sizeof(BehaviourArgs);
    if (subsystems)
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
    setBehaviour(mgr, NULL, debugbotLoop, entityKey);
}

/* XXX sample code that demonstrates the behaviour system */
/* XXX used for testing */
static void
debugbotBehaviourLoop(EntityBehaviourManager *mgr, unsigned short entityKey)
{
    mgr->state[entityKey].debugbot.i++;
    mgr->poolRef.x[entityKey]++;
    //mgr->poolRef.y[entityKey] += mgr->state[entityKey].debugbot.i;
    if (mgr->ticksDelta[entityKey] > 120)
        setBehaviour(mgr, NULL, despawnSelf, entityKey);
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

/* XXX */
static void
simpleBulletBehaviourStart(EntityBehaviourManager *mgr,
    unsigned short entityKey)
{
    struct SimpleBulletBehaviourArgs *args;
    KinematicsManager *kinMgr;

    args = mgr->args + entityKey;
    kinMgr = mgr->subsystems.debug.kinematics;
    mgr->poolRef.x[entityKey] = args->startX;
    mgr->poolRef.y[entityKey] = args->startY;
    mgr->subsystems.debug.collider->radius[entityKey] = args->radius;
    kinMgr->rot[entityKey] = args->rot;
    kinMgr->speed[entityKey] = args->speed;
    kinMgr->accel[entityKey] = 0;
    kinMgr->angularVelocity[entityKey] = 0;
    setAnimation(mgr->subsystems.debug.animator, entityKey, args->animKey, 0,
        0);
    setBehaviour(mgr, NULL, no_behaviour, entityKey);
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

