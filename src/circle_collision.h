#ifndef CIRCLE_COLLISION_H
#define CIRCLE_COLLISION_H

#include <SDL.h>

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
void drawCircCollider(SDL_Renderer *renderer, const CircleCollider *collider);

#endif
