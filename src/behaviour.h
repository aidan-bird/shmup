#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "./entity.h"
#include "./collision.h"
#include "./animation.h"

#define setEntityBehaviour(MGR, ENTITY_KEY, BEHAVIOUR_KEY) \
mgr->behaviourKey[ENTITY_KEY] = BEHAVIOUR_KEY;

typedef enum BehaviourKey BehaviourKey;
typedef struct EntityBehaviourManager EntityBehaviourManager;

typedef union BehaviourState BehaviourState;

typedef union SubsystemsList SubsystemsList;

union BehaviourState
{
    struct DebugbotState {
        int i;
    } debugbot;
};

/* XXX determine where this should be defined */
union SubsystemsList
{
    struct DebugSubsystemsList {
        Animator *animator; 
        CircleCollider *collider;
    } debug;
    struct EnemyBulletSubsystemsList {
        Animator *animator; 
        CircleCollider *circleCollider;
        CircleCollider *boxCollider;
    } enemyBullet;
};

struct EntityBehaviourManager
{
    EntityPoolRef poolRef;
    unsigned char *behaviourKey;
    unsigned short *ticksAlive;
    BehaviourState *state;
    SubsystemsList subsystems;
};

void onSpawnEvent_EntityBehaviourManager(EntityPool *caller, void *subscriber,
    const void *args);

/* XXX used for testing */
void onCollisionTestEvent_EntityBehaviourManager(void *nullptr,
    EntityBehaviourManager *mgr, const unsigned short *key);


EntityBehaviourManager *newEntityBehaviourManager(EntityPool *pool, 
    SubsystemsList *subsystems);

void deleteEntityBehaviourManager(EntityBehaviourManager *mgr);
void updateEntityBehaviourManager(EntityBehaviourManager *mgr);

enum BehaviourKey
{
    no_behaviour,
    despawnSelf,
    debugbotStart,
    debugbotLoop,
};

#endif
