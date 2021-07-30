#include <stdio.h>

#include "./event.h"

EventManager *
newEventManager(const void *parent, const char *label)
{
    EventManager *ret;

    ret = malloc(sizeof(parent));
    if (!ret)
        goto error1;
    ret->subscribers = newArray(-1, -1, sizeof(Subscriber))
    if (!ret->subscribers)
        goto error2;
    ret->parent = parent;
    ret->label = label;
    return ret;
error2:;
    free(ret);
error1:;
    return NULL;
}

// TODO make this return a malloc'ed pointer to EventManager, 
// update the rest of the functions
// int
// newEventManager(EventManager *out, const void *parent, const char *label)
// {
//     EventManager ret;
// 
//     ret.parent = parent;
//     ret.subscribers = newArray(-1, -1, sizeof(Subscriber));
//     ret.label = label;
//     if (!ret.subscribers)
//         return -1;
//     *out = ret;
//     return 0;
// }

void
raiseEvent(const EventManager mgr, const void *args)
{
    Subscriber *sub;

    if (mgr.label)
        printf("EVENT RAISED: %s\n", mgr.);
    else
        puts("ANONYMOUS EVENT RAISED");
    for (int i = 0; i < mgr.subscribers->count; i++) {
        sub = (Subscriber *)getElementArray(mgr.subscribers, i);
        sub->eventFunc(mgr.parent, sub->subscriber, args);
    }
}

/*
 * REQUIRES
 * mgr is valid
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 * deletes/invalidates mgr 
 */
void
deleteEventManager(EventManager mgr)
{
    deleteArray(mgr.subscribers);
}

/*
 * REQUIRES
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 *
 */
void
subscribeToEventManager(EventManager mgr, const void *subscriber)
{

}

