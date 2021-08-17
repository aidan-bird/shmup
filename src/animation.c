#include <stdlib.h>
#include <SDL.h>
#include <stdint.h>

#include "./entity.h"
#include "./assets.h"
#include "./animation.h"

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * animator
 *
 * EFFECTS
 * sets an entity to use the animation identified by animKey.
 */
void
setAnimation(Animator *animator, uint16_t entityKey,
    uint16_t animKey, uint8_t selector, uint8_t initialFrame)
{
    const struct AssetDefAnimation *animDef;

    /* TODO add bounds checking for selector and initialFrame for debug build */
    animDef = &animator->assetTab->assetDefTable->assetDefs[animKey].meta.anim;
    animator->animKeys[entityKey] = animKey;
    animator->selectors[entityKey] = selector;
    animator->frames[entityKey] = initialFrame;
    animator->delays[entityKey] = animDef->delays[selector][initialFrame];
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * creates a new animator from an entity pool. 
 * the animator should not outlive the pool or the assetTab.
 * returns NULL on error.
 */
Animator *
newAnimator(const EntityPool *pool, AssetTable *assetTab)
{
    Animator *ret;
    size_t vectorSize;
    size_t n;

    n = pool->count;
    vectorSize = sizeof(Animator) + n * (3 * sizeof(uint8_t)
        + sizeof(uint16_t));
    ret = malloc(vectorSize);
    if (!ret)
        return NULL;
    ret->assetTab = assetTab;
    ret->animKeys = (uint8_t *)ret + sizeof(Animator);
    ret->frames = (uint8_t *)ret->animKeys + sizeof(uint8_t) * n;
    ret->selectors = (uint8_t *)ret->frames + sizeof(uint8_t) * n;
    ret->delays = (uint16_t *)((uint8_t *)ret->selectors
        + sizeof(uint8_t) * n);
    return ret;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * animator
 *
 * EFFECTS
 * releases memory used by animator
 */
void
deleteAnimator(Animator *animator)
{
    free(animator);
}

/*
 * REQUIRES
 *
 * MODIFIES
 * animator
 *
 * EFFECTS
 * updates animator
 *
 */
void
updateAnimator(Animator *animator)
{
    size_t j;
    const struct AssetDefAnimation *animDef;
    EntityPoolIndexList list;

    list = getEntityPoolIsInitializedIndexList(animator->pool);
    for (size_t i = 0; i < list.n; i++) {
        j = list.keys[i];

        if (animator->delays[j]) {
            animator->delays[j]--;
            continue;
        } 
        /* XXX cache this in the animator? */
        animDef = &animator->assetTab->assetDefTable->assetDefs[animator->animKeys[j]].meta.anim;
        animator->frames[j] = animator->frames[j] + 1 < animDef->cols 
            ? animator->frames[j] + 1 : 0;
        animator->delays[j] = animDef->delays[animator->selectors[j]][animator->frames[j]];

    }
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * renderer
 *
 * EFFECTS
 * draws animated entities
 */
void
drawAnimator(SDL_Renderer *renderer, const Animator *animator)
{
    size_t j;
    EntityPoolIndexList list;

    list = getEntityPoolIsInitializedIndexList(animator->pool);
    for (size_t i = 0; i < list.n; i++) {
        j = list.keys[i];
        drawSprite(renderer, animator->assetTab, animator->animKeys[j],
            animator->pool->x[j], animator->pool->y[j],
            animator->selectors[j], animator->frames[j]);
    }
}

