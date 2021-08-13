#include "./shot_patterns.h"

/*
 * shot pattern mgr spawns entities (bullets) OVER TIME and at different 
 * positions/angles.
 *
 * shooter is in a Entity Pool
 *
 * Pattern Pool deals with spawning entities over time. 
 * It can live for multiple ticks.
 * It spawns entities from the reserve pool.
 * It does this by passing arguments to the behaviour subsystem attached to the
 * reserve pool.
 *
 * SUBSYSTEMS IN PLAY
 * Shooter Pool has no restrictions on its subsystems
 * Pattern Pool has the behaviour subsystem
 * Reserve Pool has the behaviour subsystem
 */

ShotPatternManager *
newShotPatternManager(EntityPool *shooterPool, 
    EntityBehaviourManager *reservePool, EntityBehaviourManager *patternPool)
{
    ShotPatternManager *ret;

    ret = malloc(sizeof(ShotPatternManager));
    if (!ret)
        goto error1;
    ret->shooterPoolRef = getEntityPoolRef(shooterPool);
    ret->reservePool = reservePool;
    ret->patternPool = patternPool;
    return ret;
error1:;
    return NULL;
}

// void
// spawnShotPattern(const ShotPatternManager *mgr, unsigned short shooterKey,
//     ShotPatternKey patternKey, const SpawnShotPatternArgs *args)
// {
// }
