#include <SDL.h>
#include <stdlib.h>

#include "./assets.h"
#include "./sdlutils.h"

typedef SDL_Texture* SpritePtr;

typedef SpritePtr (SpriteLoader)(SpriteTable*, const AssetDef*);

int
spriteLoader(SpriteTable *tab, const AssetDef *def)
{
    SDL_Texture *tmp;

    tmp = loadImage(tab->renderer, def->path);
    if (!tmp)
        return 1;
    tab->sprites[def->key] = tmp;
    return 0;
}

void
destroySpriteTable(SpriteTable *spriteTab) 
{
    for (int i = 0; i < spriteTab->count; i++) {
        SDL_DestroyTexture(spriteTab->sprites[i]);
    }
    free(spriteTab->sprites);
}

int
setupSpriteTableFromAssetDefTab(SpriteTable *spriteTab, 
    SDL_Renderer *renderer, const AssetDefTab defTable, size_t n)
{
    spriteTab->renderer = renderer;
    if (!(spriteTab->sprites = malloc(sizeof(SDL_Texture*) * n)))
        goto error1;
    for (int i = 0; i < n; i++) {
        if (spriteLoader(spriteTab, defTable + i))
            goto error2;
    }
    spriteTab->count = n;
    return 0;
error2:;
    free(spriteTab->sprites);
error1:;
    return 1;
}

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

const void*
spriteLoaderFunc(AssetLoader *loader, const char *path)
{
    return (const void *)loadImage(loader->sprite.renderer, path);
}

int
initSpriteAssetTable(AssetTable *tab, size_t n)
{
    return !(tab->assets = malloc(sizeof(void *) * n)) ? 1 : 0;
}
