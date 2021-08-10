#ifndef SHOT_PATTERN_H
#define SHOT_PATTERN_H

#include "./entity.h"
#include "./behaviour.h"

typedef struct ShotPatternManager ShotPatternManager;
typedef enum ShotPatternKey ShotPatternKey;
typedef union SpawnShotPatternArgs SpawnShotPatternArgs;

enum ShotPatternKey
{
    debugShotPattern,
};

union SpawnShotPatternArgs
{
    struct DebugSpawnShotPatternArgs {
        int i;
    } debug;
};

struct ShotPatternManager
{
    EntityPoolRef shooterPoolRef;
    EntityBehaviourManager *reservePool;
    EntityBehaviourManager *patternPool;
    /* XXX may not be needed */
    unsigned short *shooterKey;
};

ShotPatternManager *newShotPatternManager(EntityPool *shooterPool, 
    EntityBehaviourManager *reservePool, EntityBehaviourManager *patternPool);

void spawnShotPattern(const ShotPatternManager *mgr, unsigned short shooterKey,
    ShotPatternKey patternKey, const SpawnShotPatternArgs *args);

#endif
