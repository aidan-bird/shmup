#ifndef STAGE_H
#define STAGE_H

typedef struct Stage Stage;
typedef struct StageDef StageDef;
typedef int (*StageEventFunc)(Stage *stg);
typedef union StageEventData StageEventData;
typedef struct StageEventDef StageEventDef;

/*
 * used for defining the initial state of an event.
 * each struct within corosponds to a event func that requires state
 */
union StageEventData
{
    struct Delay {
        unsigned int ticks;
    } delay;
};

/*
 * used for defining an event
 * set initState to NULL for no initial state.
 */
struct StageEventDef
{
    StageEventFunc updateFunc;
    StageEventData *initState;
};

/* 
 * used for defining a stage.
 * the event after the last event must be NULL
 */
struct StageDef
{
    const char *name;
    const StageEventDef **eventDefs;
};

/*
 * represents a loaded-in stage that will be processed by the game loop
 */
struct Stage
{
    const StageDef *stgDef;
    size_t i;
    unsigned int ticks;
    StageEventData eventState;
};

int updateStage(Stage *stg);
int stageDelayFunc(Stage *stg);
Stage initStage(const StageDef *stgDef);

#endif

