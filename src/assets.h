#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>

typedef struct AssetDef AssetDef;
typedef AssetDef* AssetDefTab;

typedef struct SpriteTable SpriteTable;

int setupSpriteTableFromAssetDefTab(SpriteTable *spriteTab, SDL_Renderer *renderer, const AssetDefTab defTable, size_t n);
void destroySpriteTable(SpriteTable *spriteTab);

struct SpriteTable
{
    size_t count;
    SDL_Renderer *renderer;
    SDL_Texture **sprites;
};

// typedef struct AssetTab AssetTab;
// #define DEF_ASSET(T) \
// typedef T##* T##Ptr; \
// typedef T##Ptr (*##T##Loader)(AssetDef*); \ 
// struct AssetTab_##T
// {
//     
// };

/*
 * used for defining an asset
 */
struct AssetDef
{
    const char *path;
    int key;
};


#endif
