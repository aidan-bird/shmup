#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "./assets.h"
#include "./sdlutils.h"
#include "./utils.h"

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * tab
 *
 * EFFECTS
 * populates tab with n assets defined by defTable.
 * returns non-zero on error.
 */
int
loadAllFromAssetDefTab(AssetTable *tab, const AssetDefTab *defTab)
{
    const void *tmp;
    size_t n;
    size_t maxKey;
    size_t assetSize;

    printf("LOADING asset table: %s\n", defTab->label);
    n = defTab->count;
    maxKey = defTab->keys[0];
    for (int i = 1; i < n; i++) {
        if (maxKey < defTab->keys[i])
            maxKey = defTab->keys[i];
    }
    assetSize = MAX(n, maxKey);
    printf("asset table (%s) size will be %ld\n", defTab->label, assetSize);
    if (!(tab->assets = malloc(sizeof(void *) * assetSize)))
        goto error1;
    for (int i = 0; i < n; i++) {
        printf("LOADING asset at index %d\n", i);
        tmp = tab->loader.load(&tab->loader, defTab->assetDefs[defTab->keys[i]]
            .path);
        if (!tmp) {
            printf("FAILED to laod asset at index %d\n", i);
            tab->count = i - 1;
            goto error2;
        }
        tab->assets[defTab->keys[i]] = tmp;
    }
    tab->count = n;
    tab->assetDefTable = defTab;
    printf("DONE loading asset table: %s\n", defTab->label);
    return 0;
error2:;
    tab->destroy(tab);
error1:;
    printf("FAILED to load asset table: %s\n", defTab->label);
    return 1;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * tab
 *
 * EFFECTS
 * deletes tab
 */
void
destroySpriteAssetTable(AssetTable *tab)
{
    SDL_Texture *next;

    printf("DELETING asset table: %s\n", tab->assetDefTable->label);
    if (!tab)
        return;
    for (int i = 0; i < tab->count; i++) {
        next = ((SDL_Texture *)tab->assets[tab->assetDefTable->keys[i]]);
        if (next)
            SDL_DestroyTexture(next);
    }
    if (tab->assets)
        free(tab->assets);
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * returns a loaded-in sprite as a void*.
 * returns NULL on error.
 */
const void*
spriteLoaderFunc(AssetLoader *loader, const char *path)
{
    return (const void *)loadImage(loader->sprite.renderer, path);
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * renderer
 *
 * EFFECTS
 * draws the first column first row of a sprite sheet centered at (x,y). 
 */
void
drawSprite(SDL_Renderer *renderer, const AssetTable *spriteTab, int assetKey, 
    float x, float y)
{
    float offsetX;
    float offsetY;
    int spriteWidth;
    int spriteHeight;

    spriteWidth = spriteTab->assetDefTable->assetDefs[assetKey].meta.sprite
        .cellWidth;
    spriteHeight = spriteTab->assetDefTable->assetDefs[assetKey].meta.sprite
        .cellHeight;
    offsetX = spriteWidth / 2;
    offsetY =  spriteHeight / 2;
    SDL_Rect src = {
        .x = 0, 
        .y = 0,
        .w = spriteWidth,
        .h = spriteHeight,
    };
    SDL_Rect dest = {
        .x = (int)(x - spriteWidth / 2),
        .y = (int)(y - spriteHeight / 2),
        .w = spriteWidth,
        .h = spriteHeight,
    };
    SDL_RenderCopy(renderer, spriteTab->assets[assetKey], &src, &dest);
}

