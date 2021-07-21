#include <SDL.h>
#include <stdlib.h>

#include "./assets.h"
#include "./sdlutils.h"

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
loadAllFromAssetDefTab(AssetTable *tab, const AssetDef *defTable, 
    size_t n)
{
    const void *tmp;

    if (tab->init(tab, n))
        goto error1;
    for (int i = 0; i < n; i++) {
        tmp = tab->loader.load(&tab->loader, defTable[i].path);
        if (!tmp)
            goto error2;
        tab->assets[defTable[i].key] = tmp;
    }
    tab->count = n;
    tab->assetDefTable = defTable;
    return 0;
error2:;
    tab->destroy(tab);
error1:;
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
    for (int i = 0; i < tab->count; i++) {
        next = ((SDL_Texture *)tab->assets[tab->assetDefTable[i].key]);
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
 * tab
 *
 * EFFECTS
 * mallocs storage for tab that will store n assets.
 * returns non-zero on error.
 */
int
initSpriteAssetTable(AssetTable *tab, size_t n)
{
    return !(tab->assets = malloc(sizeof(void *) * n)) ? 1 : 0;
}
