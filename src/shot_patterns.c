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
 * Shootee Pool has no restrictions on its subsystems
 * Pattern Pool has the behaviour subsystem
 * Reserve Pool has the behaviour subsystem
 */

ShotPatternManager *
newShotPatternManager(EntityPool *shooterPool, 
    EntityBehaviourManager *reservePool, EntityBehaviourManager *patternPool)
{

    /* TODO try spawning simple bullets and forming a pattern */
    
    // size_t n;
    // ShotPatternManager *ret;

    // n = shooter->count;
    // ret = malloc(sizeof(ShotPatternManager) + sizeof(unsigned short) * n);
    // if (!ret)
    //     return NULL;
    // ret->shooterKey = (void *)ret + sizeof(ShotPatternManager);
    // ret->shooterPoolRef = getEntityPoolRef(shooterPool);
    // ret->reservePool = reservePool;
    // ret->patternPool = patternPool;
    // return ret;
}

void
spawnShotPattern(const ShotPatternManager *mgr, unsigned short shooterKey,
    ShotPatternKey patternKey, const SpawnShotPatternArgs *args)
{
    // mgr->
}
