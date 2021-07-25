#ifndef COLLISION_H
#define COLLISION_H

#include "./entity.h"

typedef struct CircleCollider CircleCollider;

struct CircleCollider
{
    size_t count;
    const size_t *activeCount;
    const char *isActive;
    const unsigned short *activeIndexMap;
    unsigned char *radius;
    const float *x;
    const float *y;
};

CircleCollider *newCircCollider(const EntityPool *pool);
void deleteCircCollider(CircleCollider *collider);
short testCircCollider(float x, float y, float r, 
    const CircleCollider *collider);

// typedef union CollisionDetectionResult CollisionDetectionResult;
// union CollisionDetectionResult
// {
//     unsigned short key;
//     struct Mult {
//         size_t count;
//         unsigned short *keys;
//     } mult;
// };

#endif
