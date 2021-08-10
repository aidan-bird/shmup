#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "./entity.h"
#include "./collision.h"
#include "./box_collision.h"
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
    /*
     * each struct corosponds to the subsystems that a particular type of 
     * entity will need
     */
    struct DebugSubsystemsList {
        Animator *animator; 
        CircleCollider *collider;
    } debug;
    struct EnemyBulletSubsystemsList {
        Animator *animator; 
        CircleCollider *circleCollider;
        /* XXX box collider not needed for this type of entity */
        BoxCollider *boxCollider;
    } enemyBullet;
    struct EnemySubsystemsList {
        Animator *animator; 
        CircleCollider *circleCollider;
        BoxCollider *boxCollider;
    } enemy;
};

struct EntityBehaviourManager
{
    EntityPoolRef poolRef;
    unsigned char *behaviourKey;
    unsigned short *ticksAlive;
    BehaviourState *state;
    SubsystemsList subsystems;
};

enum BehaviourKey
{
    no_behaviour,
    despawnSelf,
    debugbotStart,
    debugbotLoop,
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

EntityBehaviourManager *newDebugEntityBehaviourManager(EntityPool *pool,
    Animator *animator, CircleCollider *collider);

#endif
