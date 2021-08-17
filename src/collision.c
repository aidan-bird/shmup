#include <stdio.h> 

#include "./collision.h"
#include "./box_collision.h"
#include "./circle_collision.h"

/*
 * REQUIRES
 * collider is either a box or circle collider pointer.
 * entityKey is valid
 *
 * MODIFIES
 * collider
 *
 * EFFECTS
 * sets up an entity's collider from a Collider Def.
 * returns non-zero on error.
 */
int
setCollider(void *collider, const ColliderDef *def, unsigned short entityKey)
{
    switch (def->type) {
        case circle:
            ((CircleCollider *)collider)->radius[entityKey] =
                def->args.circle.radius;
            break;
        case box:
            ((BoxCollider *)collider)->width[entityKey] = def->args.box.width;
            ((BoxCollider *)collider)->height[entityKey] = 
                def->args.box.height;
            break;
        default:
            puts("setCollider(): invalid type!");
            return 1;
    }
    return 0;
}

