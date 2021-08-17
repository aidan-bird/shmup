#ifndef COLLISION_H
#define COLLISION_H

#include <stdint.h>

typedef struct ColliderDef ColliderDef;

enum ColliderDefType
{
    circle,
    box,
};

typedef enum ColliderDefType ColliderDefType;

struct ColliderDef
{
    ColliderDefType type;
    union ColliderArgs {
        struct BoxColliderArgs {
            uint8_t width;
            uint8_t height;
        } box;
        struct CircleColliderArgs {
            uint8_t radius;
        } circle;
    } args;
};

int setCollider(void *collider, const ColliderDef *def,
    uint16_t entityKey);

#endif
