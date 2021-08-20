#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <stdint.h>

#include "./entity.h"
#include "./circle_collision.h"
#include "./box_collision.h"
#include "./animation.h"
#include "./content.h"
#include "./kinematics.h"

typedef struct EntityBehaviourManager EntityBehaviourManager;
typedef union BehaviourArgs BehaviourArgs;
typedef union BehaviourState BehaviourState;
typedef union SubsystemsList SubsystemsList;

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

union BehaviourState
{
    struct DebugbotState {
        int i;
    } debugbot;
    struct DebugShotPatternState {
        float shotAngles[3];
        uint8_t burstsRemaining;
        uint8_t currentDelay;
    } debugShotPattern;
};

union BehaviourArgs
{
    struct SimpleBulletBehaviourArgs {
        float startX;
        float startY;
        float rot;
        float speed;
        uint8_t radius;
        AnimAssetKeys animKey;
    } simpleBullet;
    struct DebugShotPattern {
        uint8_t delayBetweenShots;
        AnimAssetKeys animKey;
        uint16_t targetKey;
        const EntityPool *targetPool;
        uint16_t shooterKey;
        const EntityPool *shooterPool;
        EntityBehaviourManager *ammoPoolBeh;
    } debugShotPattern;
    struct EnemyBotTest {
        struct DebugShotPattern shotArgs;
        AnimAssetKeys animKey;
        float startX;
    } enemyBotTest;
};

struct EntityBehaviourManager
{
    EntityPool *pool;
    uint8_t *behaviourKey;
    uint16_t *ticksAlive;
    uint16_t *ticksDelta;
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
    debugShotPattern_Start,
    debugShotPattern_Loop,
};

typedef enum BehaviourKey BehaviourKey;

void onSpawnEvent_EntityBehaviourManager(EntityPool *caller, 
    EntityBehaviourManager *subscriber, const uint16_t *args);
/* XXX used for testing */
void onCollisionTestEvent_EntityBehaviourManager(void *nullptr,
    EntityBehaviourManager *mgr, const uint16_t *key);
EntityBehaviourManager *newDebugEntityBehaviourManager(EntityPool *pool,
    Animator *animator, CircleCollider *collider,
    KinematicsManager *kinematics);
void deleteEntityBehaviourManager(EntityBehaviourManager *mgr);
void updateEntityBehaviourManager(EntityBehaviourManager *mgr);
void setBehaviour(EntityBehaviourManager *mgr, const BehaviourArgs *args, 
    BehaviourKey behaviourKey, uint16_t entityKey);
EntityBehaviourManager *newEntityBehaviourManager(EntityPool *,
    SubsystemsList *);
void jumpBehaviour(EntityBehaviourManager *mgr, const BehaviourArgs *args, 
    BehaviourKey behaviourKey, uint16_t entityKey);

#endif
