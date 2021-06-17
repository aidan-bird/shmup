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
