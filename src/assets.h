#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>

typedef struct AssetDef AssetDef;
typedef struct AssetDefTab AssetDefTab;

typedef struct AssetTable AssetTable;
typedef struct AssetLoader AssetLoader;
typedef const void* (*AssetLoaderFunc)(AssetLoader*, const char *);
typedef void (*AssetTabDestroyFunc)(AssetTable*);
typedef int (*AssetTabInitFunc)(AssetTable*, size_t);
typedef union AssetMetadata AssetMetadata;

int initSpriteAssetTable(AssetTable *tab, size_t n);
void destroySpriteAssetTable(AssetTable *tab);

int loadAllFromAssetDefTab(AssetTable *tab, const AssetDefTab *defTab);

void
drawSprite(SDL_Renderer *renderer, const AssetTable *spriteTab, int assetKey, 
    float x, float y, unsigned char row, unsigned char col);

const void* spriteLoaderFunc(AssetLoader *loader, const char *path);

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
    AssetTabDestroyFunc destroy;
    AssetLoader loader;
    const AssetDefTab *assetDefTable;
    const void **assets;
};

union AssetMetadata {
    struct AssetDefSprite {
        unsigned short cellWidth;
        unsigned short cellHeight;
        unsigned char rows;
        unsigned char cols;
    } sprite;
};

/*
 * used for defining an asset
 */
struct AssetDef
{
    const char *path;
    int key;
    AssetMetadata meta;
};

/*
 * used for defining a list of assets
 */
struct AssetDefTab
{
    const char *label;
    size_t count;
    const int *keys;
    const AssetDef *assetDefs;
};

#endif
