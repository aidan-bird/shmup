#include <stdio.h>

#include "./stage.h"
#include "./debug.h"

int
stageDelayFunc(Stage *stg)
{
    if (!stg->ticks) {
        printf("stage %s: stageDelayFunc() START! tick delay is %d\n", 
            stg->stgDef->name, stg->eventState.delay.ticks);
    }
    if (stg->eventState.delay.ticks) {
        stg->eventState.delay.ticks--;
        return 0;
    }
    printf("stage %s: stageDelayFunc() END!\n", stg->stgDef->name);
    return 1;
}

/*
 * REQUIRES
 * the event after the last event of stgDef is NULL.
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * Returns a stage using a stage def.
 */
Stage
initStage(const StageDef *stgDef)
{
    Stage ret;

    printf("LOADED stage: %s\n", stgDef->name);
    ret.i = 0;
    ret.ticks = 0;
    ret.stgDef = stgDef;
    if (stgDef->eventDefs[0]->initState)
        ret.eventState = *(stgDef->eventDefs[0]->initState);
    return ret;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * stg
 *
 * EFFECTS
 * updates a stage.
 * returns non-zero when the last event finishes.
 */
int
updateStage(Stage *stg)
{
    if (!stg->stgDef->eventDefs[stg->i])
        return 1;
    if (stg->stgDef->eventDefs[stg->i]->updateFunc(stg)) {
        stg->i++;
        stg->ticks = 0;
        if (!stg->stgDef->eventDefs[stg->i])
            return 1;
        if (stg->stgDef->eventDefs[stg->i]->initState)
            stg->eventState = *(stg->stgDef->eventDefs[stg->i]->initState);
    }
    stg->ticks++;
    return 0;
}

