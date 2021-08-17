#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include <stdint.h>

#include "./assets.h"
#include "./entity.h"

typedef struct AnimDef AnimDef;
typedef struct Animator Animator;

Animator *newAnimator(EntityPool *pool, AssetTable *assetTab);
void deleteAnimator(Animator *animator);
void updateAnimator(Animator *animator);
void drawAnimator(SDL_Renderer *renderer, const Animator *animator);
void setAnimation(Animator *animator, uint16_t entityKey, 
    uint16_t animKey, uint8_t selector,
    uint8_t initialFrame);

struct Animator
{
    EntityPool *pool;
    AssetTable *assetTab;
    uint8_t *animKeys;
    uint8_t *frames;
    uint8_t *selectors;
    uint16_t *delays;
};

#endif
