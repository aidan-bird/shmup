#ifndef BOX_COLLISION_H
#define BOX_COLLISION_H

#include <SDL.h>

#include "./entity.h"

typedef struct BoxCollider BoxCollider;

struct BoxCollider
{
    EntityPoolRef poolRef;
    unsigned char *width;
    unsigned char *height;
};

BoxCollider *newBoxCollider(const EntityPool *pool);
void deleteBoxCollider(BoxCollider *collider);
short testBoxCollider(float x, float y, unsigned width, unsigned height,
    const BoxCollider *collider);
void drawBoxCollider(SDL_Renderer *renderer, const BoxCollider *collider);

#endif
