#ifndef BOX_COLLISION_H
#define BOX_COLLISION_H

#include <SDL.h>
#include <stdint.h>

#include "./entity.h"

typedef struct BoxCollider BoxCollider;

struct BoxCollider
{
    EntityPool *pool;
    uint8_t *width;
    uint8_t *height;
};

BoxCollider * newBoxCollider(EntityPool *pool);
void deleteBoxCollider(BoxCollider *collider);
int32_t testBoxCollider(float x, float y, const BoxCollider *collider);
void drawBoxCollider(SDL_Renderer *renderer, const BoxCollider *collider);

#endif
