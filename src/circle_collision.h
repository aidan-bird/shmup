#ifndef CIRCLE_COLLISION_H
#define CIRCLE_COLLISION_H

#include <SDL.h>
#include <stdint.h>

#include "./entity.h"

typedef struct CircleCollider CircleCollider;

struct CircleCollider
{
    EntityPool *pool;
    uint8_t *radius;
};

CircleCollider *newCircCollider(EntityPool *pool);
void deleteCircCollider(CircleCollider *collider);
uint32_t testCircCollider(float x, float y, float r,
    const CircleCollider *collider);
void drawCircCollider(SDL_Renderer *renderer, CircleCollider *collider);

#endif
