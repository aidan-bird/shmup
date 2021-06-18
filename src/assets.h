#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>

typedef struct AssetDef AssetDef;
typedef AssetDef* AssetDefTab;

typedef struct SpriteTable SpriteTable;

typedef struct AssetTable AssetTable;
typedef struct AssetLoader AssetLoader;
typedef const void* (*AssetLoaderFunc)(AssetLoader*, const char *);
typedef void (*AssetTabDestroyFunc)(AssetTable*);
typedef int (*AssetTabInitFunc)(AssetTable*, size_t);

int setupSpriteTableFromAssetDefTab(SpriteTable *spriteTab, 
    SDL_Renderer *renderer, const AssetDefTab defTable, size_t n);
void destroySpriteTable(SpriteTable *spriteTab);

int initSpriteAssetTable(AssetTable *tab, size_t n);
void destroySpriteAssetTable(AssetTable *tab);
int loadAllFromAssetDefTab(AssetTable *tab, const AssetDef *defTable, 
    size_t n);

const void* spriteLoaderFunc(AssetLoader *loader, const char *path);

struct SpriteTable
{
    size_t count;
    SDL_Renderer *renderer;
    SDL_Texture **sprites;
};


struct AssetLoader
{
    AssetLoaderFunc load;
    union {
        /* for loading in sprites */
        struct AssetLoaderSprite {
            SDL_Renderer *renderer;
        } sprite;
    };
};


/*
 * for querying loaded in assets
 */
struct AssetTable
{
    size_t count;
    AssetTabInitFunc init;
    AssetTabDestroyFunc destroy;
    AssetLoader loader;
    const AssetDef *assetDefTable;
    const void **assets;
};


/*
 * used for defining an asset
 */
struct AssetDef
{
    const char *path;
    int key;
};


#endif
