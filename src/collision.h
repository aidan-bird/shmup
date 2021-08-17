#ifndef COLLISION_H
#define COLLISION_H

typedef struct ColliderDef ColliderDef;
typedef enum ColliderDefType ColliderDefType;

enum ColliderDefType
{
    circle,
    box,
};

struct ColliderDef
{
    ColliderDefType type;
    union ColliderArgs {
        struct BoxColliderArgs {
            unsigned char width;
            unsigned char height;
        } box;
        struct CircleColliderArgs {
            unsigned char radius;
        } circle;
    } args;
};

int setCollider(void *collider, const ColliderDef *def,
    unsigned short entityKey);

#endif
