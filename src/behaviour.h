#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "./entity.h"
#include "./collision.h"
#include "./box_collision.h"
#include "./animation.h"
#include "./content.h"
#include "./kinematics.h"

typedef enum BehaviourKey BehaviourKey;
typedef struct EntityBehaviourManager EntityBehaviourManager;
typedef union BehaviourArgs BehaviourArgs;
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
        KinematicsManager *kinematics;
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

union BehaviourArgs
{
    struct SimpleBulletBehaviourArgs {
        float startX;
        float startY;
        float rot;
        float speed;
        unsigned char radius;
        AnimAssetKeys animKey;
    } simpleBullet;
};

struct EntityBehaviourManager
{
    EntityPoolRef poolRef;
    unsigned char *behaviourKey;
    unsigned short *ticksAlive;
    unsigned short *ticksDelta;
    BehaviourState *state;
    BehaviourArgs *args;
    SubsystemsList subsystems;
};

enum BehaviourKey
{
    no_behaviour,
    despawnSelf,
    debugbotStart,
    debugbotLoop,
    simpleBulletStart,
};

void onSpawnEvent_EntityBehaviourManager(EntityPool *caller, 
    EntityBehaviourManager *subscriber, const unsigned short *args);
/* XXX used for testing */
void onCollisionTestEvent_EntityBehaviourManager(void *nullptr,
    EntityBehaviourManager *mgr, const unsigned short *key);
EntityBehaviourManager *newDebugEntityBehaviourManager(EntityPool *pool,
    Animator *animator, CircleCollider *collider,
    KinematicsManager *kinematics);
void deleteEntityBehaviourManager(EntityBehaviourManager *mgr);
void updateEntityBehaviourManager(EntityBehaviourManager *mgr);
void setBehaviour(EntityBehaviourManager *mgr, const BehaviourArgs *args, 
    BehaviourKey behaviourKey, unsigned short entityKey);

#endif
