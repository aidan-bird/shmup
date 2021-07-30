#ifndef EVENT_H
#define EVENT_H

#include "../alib/src/array.h"

typedef struct EventManager EventManager;
typedef struct Subscriber Subscriber;

typedef void (*OnEventFunc)(void *parent, void *subscriber, const void *args);

struct Subscriber
{
    void *subscriber;
    OnEventFunc eventFunc;
    const char *label;
};

struct EventManager
{
    void *parent; 
    Array *subscribers;
    const char *label;
};

EventManager *newEventManager(void *parent, const char *label);
void raiseEvent(const EventManager *mgr, const void *args);
void deleteEventManager(EventManager *mgr);
int subscribeToEventManager(EventManager *mgr, void *subscriberObject, 
    OnEventFunc eventFunc, const char *label);

#endif
