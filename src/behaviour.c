#include <stdlib.h>
#include <math.h>

#include "./behaviour.h"
#include "./entity.h"
#include "./config.h"
#include "./content.h"
#include "./kinematics.h"
#include "./utils.h"
#include "./debug.h"

typedef void (*BehaviourUpdateFunc)(EntityBehaviourManager *, uint16_t);

static void debugbotBehaviourStart(EntityBehaviourManager *, uint16_t);
static void debugbotBehaviourLoop(EntityBehaviourManager *, uint16_t);
static void despawnSelfStart(EntityBehaviourManager *, uint16_t);
static void simpleBulletBehaviourStart(EntityBehaviourManager *,
    uint16_t);
static void debugShotPatternStart(EntityBehaviourManager *, uint16_t);
static void debugShotPatternLoop(EntityBehaviourManager *, uint16_t);

/*
 * XXX store the pointer instead of a key
 */

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
    [debugShotPattern_Start] = debugShotPatternStart,
    [debugShotPattern_Loop] = debugShotPatternLoop,
};

/*
 * REQUIRES
 * all parameters are valid.
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 * sets the behaviour of an entity.
 * the behaviour will start on the next tick.
 * args is optional.
 */
void
setBehaviour(EntityBehaviourManager *mgr, const BehaviourArgs *args, 
    BehaviourKey behaviourKey, uint16_t entityKey)
{
    mgr->behaviourKey[entityKey] = behaviourKey;
    mgr->ticksDelta[entityKey] = 0;
    if (args)
        mgr->args[entityKey] = *args;
}

/*
 * REQUIRES
 * all parameters are valid.
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 * sets the behaviour of an entity.
 * the behaviour will start when this is called.
 * args is optional.
 */
void
jumpBehaviour(EntityBehaviourManager *mgr, const BehaviourArgs *args, 
    BehaviourKey behaviourKey, uint16_t entityKey)
{
    mgr->behaviourKey[entityKey] = behaviourKey;
    mgr->ticksDelta[entityKey] = 0;
    if (args)
        mgr->args[entityKey] = *args;
    if (behaviourKey == no_behaviour)
        return;
    behaviourTab[behaviourKey](mgr, entityKey);
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
    EntityBehaviourManager *subscriber, const uint16_t *args)
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
    vectorSize = sizeof(uint8_t) + 2 * sizeof(uint16_t) 
        + sizeof(uint32_t) + sizeof(BehaviourState) + sizeof(BehaviourArgs);
    ret = malloc(sizeof(EntityBehaviourManager) + n * vectorSize);
    if (!ret)
        goto error1;
    ret->pool = pool;
    ret->behaviourKey = (uint8_t *)ret + sizeof(EntityBehaviourManager);
    ret->ticksAlive = (uint16_t *)((uint8_t *)ret->behaviourKey
        + n * sizeof(uint8_t));
    ret->state = (BehaviourState *)((uint8_t *)ret->ticksAlive
        + n * sizeof(uint16_t));
    ret->args = (BehaviourArgs *)((uint8_t *)ret->state
        + n * sizeof(BehaviourState));
    ret->ticksDelta = (uint16_t *)((uint8_t *)ret->args
        + n * sizeof(BehaviourArgs));
    if (subsystems)
        ret->subsystems = *subsystems;
    /* subscribe to onSpawnEntity Event*/
    if (subscribeToEventManager(pool->onSpawnEntityEvent, ret,
        (OnEventFunc)onSpawnEvent_EntityBehaviourManager,
        "onSpawnEvent_EntityBehaviourManager"))
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
 * releases the memory used by mgr
 */
void
deleteEntityBehaviourManager(EntityBehaviourManager *mgr)
{
    if (!mgr)
        return;
    free(mgr);
    despawnedObjectsCount++;
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
    EntityPoolIndexList list;

    list = getEntityPoolActiveIndexList(mgr->pool);
    for (size_t i = 0; i < list.n; i++) {
        j = list.keys[i];
        mgr->ticksAlive[j]++;
        if (mgr->behaviourKey[j] == no_behaviour)
            continue;
        behaviourTab[mgr->behaviourKey[j]](mgr, j);
    }
}

/* XXX sample code that demonstrates the behaviour system */
static void
debugbotBehaviourStart(EntityBehaviourManager *mgr, uint16_t entityKey)
{
    mgr->state[entityKey].debugbot.i = 0;
    mgr->pool->x[entityKey] = WIDTH / 2;
    mgr->pool->y[entityKey] = HEIGHT / 2;
    mgr->subsystems.debug.collider->radius[entityKey] = 32;
    setAnimation(mgr->subsystems.debug.animator, entityKey, debuganim, 0, 0);
    raiseIsInitializedFlag(mgr->pool, entityKey);
    jumpBehaviour(mgr, NULL, debugbotLoop, entityKey);
}

/* XXX sample code that demonstrates the behaviour system */
static void
debugbotBehaviourLoop(EntityBehaviourManager *mgr, uint16_t entityKey)
{
    mgr->state[entityKey].debugbot.i++;
    mgr->pool->x[entityKey]++;
    //mgr->poolRef.y[entityKey] += mgr->state[entityKey].debugbot.i;
    if (mgr->ticksDelta[entityKey] > 120)
        setBehaviour(mgr, NULL, despawnSelf, entityKey);
}

static void
despawnSelfStart(EntityBehaviourManager *mgr, uint16_t entityKey)
{
    despawnEntity(mgr->pool, entityKey);
}

/* XXX used for testing */
void
onCollisionTestEvent_EntityBehaviourManager(void *nullptr,
    EntityBehaviourManager *mgr, const uint16_t *key)
{
    mgr->behaviourKey[*key] = despawnSelf;
}

/* XXX */
static void
simpleBulletBehaviourStart(EntityBehaviourManager *mgr,
    uint16_t entityKey)
{
    const struct SimpleBulletBehaviourArgs *args;
    KinematicsManager *kinMgr;

    args = (struct SimpleBulletBehaviourArgs *)(mgr->args + entityKey);
    kinMgr = mgr->subsystems.debug.kinematics;
    mgr->pool->x[entityKey] = args->startX;
    mgr->pool->y[entityKey] = args->startY;
    mgr->subsystems.debug.collider->radius[entityKey] = args->radius;
    kinMgr->rot[entityKey] = args->rot;
    kinMgr->speed[entityKey] = args->speed;
    kinMgr->accel[entityKey] = 0;
    kinMgr->angularVelocity[entityKey] = 0;
    setAnimation(mgr->subsystems.debug.animator, entityKey, args->animKey, 0,
        0);
    raiseIsInitializedFlag(mgr->pool, entityKey);
    jumpBehaviour(mgr, NULL, no_behaviour, entityKey);
}

/* XXX shot pattern used for testing */
static void
debugShotPatternStart(EntityBehaviourManager *mgr, uint16_t entityKey)
{
    const struct DebugShotPattern *args;
    struct DebugShotPatternState *state;
    float shotAngleOffset = PI_F / 12;
    float baseShotAngle;

    state = (struct DebugShotPatternState *)mgr->state + entityKey;
    args =  (const struct DebugShotPattern*)mgr->args + entityKey;
    baseShotAngle = atan2f(args->shooterPool->x[args->shooterKey] 
        - args->targetPool->x[args->targetKey],
        args->shooterPool->y[args->shooterKey] 
        - args->targetPool->y[args->targetKey]) - shotAngleOffset;
    for (size_t i = 0; i < LEN(state->shotAngles); i++)
        state->shotAngles[i] = baseShotAngle + i * shotAngleOffset;
    state->burstsRemaining = 3;
    state->currentDelay = 0;
    jumpBehaviour(mgr, NULL, debugShotPattern_Loop, entityKey);
}

static void
debugShotPatternLoop(EntityBehaviourManager *mgr, uint16_t entityKey)
{
    BehaviourArgs spawnArgs;
    const struct DebugShotPattern *args;
    struct DebugShotPatternState *state;
    uint16_t key;

    state = (struct DebugShotPatternState *)(mgr->state + entityKey);
    if (!state->burstsRemaining)
        setBehaviour(mgr, NULL, despawnSelf, entityKey);
    if (state->currentDelay) {
        state->currentDelay--;
        return;
    }
    args = (const struct DebugShotPattern *)(mgr->args + entityKey);
    spawnArgs = (BehaviourArgs) {
        .simpleBullet = {
            .startX = args->shooterPool->x[args->shooterKey],
            .startY = args->shooterPool->y[args->shooterKey],
            .speed = 5,
            .radius = 32,
            .animKey = args->animKey,
        },
    };
    for (size_t i = 0; i < LEN(state->shotAngles); i++) {
        spawnArgs.simpleBullet.rot = state->shotAngles[i];
        key = spawnEntity(args->ammoPoolBeh->pool);
        setBehaviour(args->ammoPoolBeh, &spawnArgs, simpleBulletStart, key);
    }
    state->burstsRemaining--;
    state->currentDelay = args->delayBetweenShots;
}

/* XXX enemy bot for testing */
/*
 * XXX testing an enemy entity
 *
 * this entity will be spawned by the stage system after a specified delay 
 * from starting the game. The entity will attack the player by spawning
 * the following bullet pattern. Should the entity live and expend all of its
 * ammo, it shall exit the stage by accelerating to either the left or right of
 * the stage until it goes past the boundaries of the game. then it will 
 * de-spawn itself.
 */
static void
enemyBotTestStart(EntityBehaviourManager *mgr, uint16_t entityKey)
{
}

static void
enemyBotTestLoop(EntityBehaviourManager *mgr, uint16_t entityKey)
{
}

