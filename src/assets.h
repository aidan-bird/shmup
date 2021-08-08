#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>

typedef struct AssetDef AssetDef;
typedef struct AssetDefTab AssetDefTab;
typedef struct AssetTable AssetTable;
typedef struct AssetLoader AssetLoader;
typedef union AssetMetadata AssetMetadata;
typedef enum AssetLoaderStatus AssetLoaderStatus;

typedef const void* (*AssetLoaderFunc)(AssetLoader*, const char *);
typedef void (*AssetTabDestroyFunc)(AssetTable*);
typedef int (*AssetTabInitFunc)(AssetTable*, size_t);

int initSpriteAssetTable(AssetTable *tab, size_t n);
void destroySpriteAssetTable(AssetTable *tab);
int loadAllFromAssetDefTab(AssetTable *tab, const AssetDefTab *defTab);
void
drawSprite(SDL_Renderer *renderer, const AssetTable *spriteTab, int assetKey, 
    float x, float y, unsigned char row, unsigned char col);
const void *spriteLoaderFunc(AssetLoader *loader, const char *path);
const void *animLoaderFunc(AssetLoader *loader, const char *path);
void destroyAnimAssetTable(AssetTable *tab);
int isAssetLoaded(const AssetTable *tab, unsigned assetKey);

enum AssetLoaderStatus
{
    loaded = 0,
    not_loaded = 1,
    invalid = 2,
};

struct AssetLoader
{
    AssetLoaderFunc load;
    union {
        /* for loading in sprites */
        struct AssetLoaderSprite {
            SDL_Renderer *renderer;
        } sprite;
        struct AssetLoaderAnimation {
            SDL_Renderer *renderer;
        } anim;
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
    AssetLoaderStatus *loaderStatus;
};

union AssetMetadata {
    struct AssetDefSprite {
        unsigned short cellWidth;
        unsigned short cellHeight;
        unsigned char rows;
        unsigned char cols;
    } sprite;
    struct AssetDefAnimation {
        unsigned short cellWidth;
        unsigned short cellHeight;
        unsigned char rows;
        unsigned char cols;
        const unsigned short **delays;
    } anim;
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
