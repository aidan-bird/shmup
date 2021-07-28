#ifndef COLLISION_H
#define COLLISION_H

#include "./entity.h"

typedef struct CircleCollider CircleCollider;

struct CircleCollider
{
    EntityPoolRef poolRef;
    unsigned char *radius;
};

CircleCollider *newCircCollider(const EntityPool *pool);
void deleteCircCollider(CircleCollider *collider);
short testCircCollider(float x, float y, float r, 
    const CircleCollider *collider);

#endif
