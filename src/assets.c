#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./assets.h"
#include "./sdlutils.h"
#include "./utils.h"
#include "./debug.h"

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * prints a message about the load status of an asset identified by assetKey.
 * returns non-zero if the asset identified by assetKey is loaded.
 * returns zero otherwise.
 */
static int
statAssetKey(const AssetTable *tab, uint32_t assetKey)
{
    if (assetKey >= tab->count) {
        puts("statAssetKey: key is INVALID and OUT OF BOUNDS");
        return 0;
    }
    switch (tab->loaderStatus[assetKey])
    {
        case loaded:
            // puts("statAssetKey: key is VALID");
            return 1;
        case not_loaded:
            puts("statAssetKey: key is VALID but NOT LOADED");
            break;
        case invalid:
            puts("statAssetKey: key is NOT VALID");
            break;
    }
    return 0;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * tab
 *
 * EFFECTS
 * marks all elements in tab as either invalid if the element is not 
 * referenced by a key in defTab, or not_loaded otherwise.
 */
static void
setupLoaderStatusTable(AssetLoaderStatus* tab, size_t n,
    const AssetDefTab *defTab)
{
    for (size_t i = 0; i < n; i++)
        tab[i] = invalid;
    for (size_t i = 0; i < defTab->count; i++)
        tab[defTab->keys[i]] = not_loaded;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * returns non-zero if the asset identified by assetKey is loaded.
 * returns zero otherwise.
 */
int
isAssetLoaded(const AssetTable *tab, uint32_t assetKey)
{
    return statAssetKey(tab, assetKey);
    /* TODO check with no print messages */
    // if (assetKey >= tab->count || tab->loaderStatus[assetKey] != loaded)
    //     return 0;
}

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
loadAllFromAssetDefTab(AssetTable *tab, AssetDefTab *defTab)
{
    void *tmp;
    size_t n;
    size_t maxKey;
    size_t assetSize;
    size_t vectorSize;
    size_t j;

    vectorSize = sizeof(void *) + sizeof(AssetLoaderStatus);
    printf("LOADING asset table: %s\n", defTab->label);
    n = defTab->count;
    /* get the max key */
    maxKey = defTab->keys[0];
    for (size_t i = 1; i < n; i++) {
        if (maxKey < defTab->keys[i])
            maxKey = defTab->keys[i];
    }
    assetSize = MAX(n, maxKey);
    printf("asset table (%s) size will be %ld\n", defTab->label, assetSize);
    if (!(tab->assets = malloc(vectorSize * assetSize)))
        goto error1;
    /* construct loader status table */
    tab->loaderStatus = (AssetLoaderStatus *)((uint8_t *)tab->assets
        + sizeof(void *) * assetSize);
    setupLoaderStatusTable(tab->loaderStatus, assetSize, defTab);
    /* load assets */
    for (size_t i = 0; i < n; i++) {
        j = defTab->keys[i];
        printf("LOADING asset at index %ld\n", i);
        tmp = tab->loader.load(&tab->loader, defTab->assetDefs[j].path);
        if (!tmp) {
            printf("FAILED to laod asset at index %ld\n", i);
            tab->count = i - 1;
            goto error2;
        }
        tab->assets[j] = tmp;
        tab->loaderStatus[j] = loaded;
        spawnedObjectsCount++;
    }
    tab->count = n;
    tab->assetDefTable = defTab;
    printf("DONE loading asset table: %s\n", defTab->label);
    spawnedObjectsCount++;
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

    if (!tab)
        return;
    printf("DELETING asset table: %s\n", tab->assetDefTable->label);
    if (!tab)
        return;
    for (size_t i = 0; i < tab->count; i++) {
        next = ((SDL_Texture *)tab->assets[tab->assetDefTable->keys[i]]);
        if (next) {
            despawnedObjectsCount++;
            SDL_DestroyTexture(next);
        }
    }
    if (tab->assets) {
        free(tab->assets);
        despawnedObjectsCount++;
    }
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
void *
spriteLoaderFunc(AssetLoader *loader, const char *path)
{
    return (void *)loadImage(loader->args.sprite.renderer, path);
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * renderer
 *
 * EFFECTS
 * draws a sprite from a sprite sheet and center it at (x,y). 
 */
void
drawSprite(SDL_Renderer *renderer, AssetTable *spriteTab, int assetKey, 
    float x, float y, uint8_t row, uint8_t col)
{
    int spriteWidth;
    int spriteHeight;

    /* 
     * XXX
     * check if asset key is valid.
     * this may not be needed if atleast one update call occurs before any 
     * draw calls.
     * otherwise it is possible for draw calls to occurs on invalid keys
     */
    // if (!isAssetLoaded(spriteTab, assetKey))
    //     return;
    spriteWidth = spriteTab->assetDefTable->assetDefs[assetKey].meta.sprite
        .cellWidth;
    spriteHeight = spriteTab->assetDefTable->assetDefs[assetKey].meta.sprite
        .cellHeight;
    SDL_Rect src = {
        .x = spriteWidth * col,
        .y = spriteHeight * row,
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

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * renderer
 *
 * EFFECTS
 * loads a sprite sheet from a AssetDefTab
 * returns NULL on error
 */
AssetTable *
loadSpriteSheet(AssetDefTab *spriteSheetDef, SDL_Renderer *renderer)
{
    AssetTable *ret;

    ret = malloc(sizeof(AssetTable));
    if (!ret)
        goto error1;
    *ret = (AssetTable) {
        .destroy = destroySpriteAssetTable,
            .loader = (AssetLoader) {
                .load = spriteLoaderFunc,
                .args.sprite = {
                    .renderer = renderer,
                },
            },
        .assetDefTable = NULL,
    };
    if (loadAllFromAssetDefTab(ret, spriteSheetDef))
        goto error2;
    return ret;
error2:;
    free(ret);
error1:;
    return NULL;
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
deleteAssetTable(AssetTable *tab)
{
    if (!tab)
        return;
    if (tab->destroy)
        tab->destroy(tab);
    free(tab);
    despawnedObjectsCount++;
}


