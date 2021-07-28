#ifndef ANIMATION_H
#define ANIMATION_H


typedef struct AnimDef AnimDef;
typedef struct Animation Animation;

/* 
 * Used for defining an animation.
 * An animation shall be a sequence of sprites, each of which has an 
 * associated tick delay. Animation will change to the next sprite when the 
 * tick delay has lapsed. An animation key will be associated with each entity,
 * and it will select that animation is being used.
 */
struct AnimDef
{

};

struct Animation
{
    size_t count;
    const size_t *activeCount;
    const char *isActive;
    const unsigned short *activeIndexMap;
    const float *x;
    const float *y;
};

#endif
