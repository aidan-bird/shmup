#include <stdlib.h>
#include <SDL.h>

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
setAnimation(Animator *animator, unsigned short entityKey,
    unsigned short animKey, unsigned char selector, unsigned char initialFrame)
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
newAnimator(const EntityPool *pool, const AssetTable *assetTab)
{
    Animator *ret;
    size_t vectorSize;
    size_t n;

    n = pool->count;
    vectorSize = 3 * sizeof(unsigned char) + sizeof(unsigned short);
    ret = malloc(sizeof(Animator) + vectorSize * n);
    if (!ret)
        return NULL;
    ret->assetTab = assetTab;
    ret->poolRef = getEntityPoolRef(pool);
    /* XXX void pointer arithmetic is not portable */
    ret->animKeys = (void *)ret + sizeof(Animator);
    ret->frames = (void *)ret->animKeys + sizeof(unsigned char) * n;
    ret->selectors = (void *)ret->frames + sizeof(unsigned char) * n;
    ret->delays = (void *)ret->selectors + sizeof(unsigned char) * n;
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

    for (int i = 0; i < *animator->poolRef.activeCount; i++) {
        j = animator->poolRef.activeIndexMap[i];
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
 *
 * MODIFIES
 *
 * EFFECTS
 *
 */
void
drawAnimator(SDL_Renderer *renderer, const Animator *animator)
{
    size_t j;
    EntityPoolActiveIndexList list;

    list = getEntityPoolActiveIndexList(animator->poolRef.pool);
    for (int i = 0; i < list.n; i++) {
        j = list.keys[i];
        drawSprite(renderer, animator->assetTab, animator->animKeys[j],
            animator->poolRef.x[j], animator->poolRef.y[j],
            animator->selectors[j], animator->frames[j]);
    }
}

