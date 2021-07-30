#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "./entity.h"

#define setEntityBehaviour(MGR, ENTITY_KEY, BEHAVIOUR_KEY) \
mgr->behaviourKey[ENTITY_KEY] = BEHAVIOUR_KEY;

typedef enum BehaviourKey BehaviourKey;
typedef struct EntityBehaviourManager EntityBehaviourManager;

struct EntityBehaviourManager
{
    EntityPoolRef poolRef;
    unsigned char *behaviourKey;
    unsigned short *ticksAlive;
    unsigned *flags;
};

void onSpawnEvent_EntityBehaviourManager(EntityPool *caller, void *subscriber,
    const void *args);

EntityBehaviourManager *newEntityBehaviourManager(EntityPool *pool);
void deleteEntityBehaviourManager(EntityBehaviourManager *mgr);
void updateEntityBehaviourManager(EntityBehaviourManager *mgr);

enum BehaviourKey
{
    no_behaviour,
    debugbot,
};

#endif
