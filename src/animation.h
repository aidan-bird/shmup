#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>

#include "./assets.h"
#include "./entity.h"

typedef struct AnimDef AnimDef;
typedef struct Animator Animator;

typedef enum AnimationKey AnimationKey;

Animator *newAnimator(const EntityPool *pool, const AssetTable *assetTab);
void deleteAnimator(Animator *animator);
void updateAnimator(Animator *animator);
void drawAnimator(SDL_Renderer *renderer, const Animator *animator);
void setAnimation(Animator *animator, unsigned short entityKey, 
    unsigned short animKey, unsigned char selector,
    unsigned char initialFrame);

struct Animator
{
    EntityPoolRef poolRef;
    const AssetTable *assetTab;
    unsigned char *animKeys;
    unsigned char *frames;
    unsigned char *selectors;
    unsigned short *delays;
};

#endif
