#include <stdio.h>
#include <stdlib.h>

#include "./event.h"

/*
 * REQUIRES
 * all pointer arguments are valid 
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * Returns a new EventManager.
 * Returns null on error.
 */
EventManager *
newEventManager(void *parent, const char *label)
{
    EventManager *ret;

    ret = malloc(sizeof(parent));
    if (!ret)
        goto error1;
    ret->subscribers = newArray(-1, -1, sizeof(Subscriber));
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

void
raiseEvent(const EventManager *mgr, const void *args)
{
    Subscriber *sub;

    if (mgr->label)
        printf("EVENT RAISED: %s\n", mgr->label);
    else
        puts("ANONYMOUS EVENT RAISED");
    for (size_t i = 0; i < mgr->subscribers->count; i++) {
        sub = (Subscriber *)getElementArray(mgr->subscribers, i);
        printf("Notifying: %s\n", sub->label == NULL ? "ANONYMOUS" : sub->label);
        sub->eventFunc(mgr->parent, sub->subscriber, args);
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
deleteEventManager(EventManager *mgr)
{
    printf("DELETING %s EventManager\n", mgr->label == NULL ? "ANONYMOUS" : 
        mgr->label);
    deleteArray(mgr->subscribers);
    free(mgr);
}

/*
 * REQUIRES
 * all pointer parameters are valid
 *
 * MODIFIES
 * mgr
 *
 * EFFECTS
 * Subscribes to an Event Manager.
 * returns non-zero on error.
 */
int
subscribeToEventManager(EventManager *mgr, void *subscriberObject, 
    OnEventFunc eventFunc, const char *label)
{
    Subscriber sub;

    sub = (Subscriber) {
        .subscriber = subscriberObject,
        .eventFunc = eventFunc,
        .label = label,
    };
    return tryPushArray(&mgr->subscribers, &sub) == NULL;
}

