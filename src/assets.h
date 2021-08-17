#ifndef ASSETS_H
#define ASSETS_H

#include <SDL.h>
#include <stdint.h>

typedef struct AssetDef AssetDef;
typedef struct AssetDefTab AssetDefTab;
typedef struct AssetTable AssetTable;
typedef struct AssetLoader AssetLoader;
typedef union AssetMetadata AssetMetadata;

typedef void* (*AssetLoaderFunc)(AssetLoader*, const char *);
typedef void (*AssetTabDestroyFunc)(AssetTable*);
typedef int (*AssetTabInitFunc)(AssetTable*, size_t);

enum AssetLoaderStatus
{
    loaded = 0,
    not_loaded = 1,
    invalid = 2,
};

typedef enum AssetLoaderStatus AssetLoaderStatus;

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
    } args;
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
    void **assets;
    AssetLoaderStatus *loaderStatus;
};

union AssetMetadata {
    struct AssetDefSprite {
        uint16_t cellWidth;
        uint16_t cellHeight;
        uint8_t rows;
        uint8_t cols;
    } sprite;
    struct AssetDefAnimation {
        uint16_t cellWidth;
        uint16_t cellHeight;
        uint8_t rows;
        uint8_t cols;
        const uint16_t **delays;
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
    const size_t *keys;
    const AssetDef *assetDefs;
};

int initSpriteAssetTable(AssetTable *tab, size_t n);
void destroySpriteAssetTable(AssetTable *tab);
int loadAllFromAssetDefTab(AssetTable *tab, AssetDefTab *defTab);
void
drawSprite(SDL_Renderer *renderer, AssetTable *spriteTab, int assetKey, 
    float x, float y, uint8_t row, uint8_t col);
void *spriteLoaderFunc(AssetLoader *loader, const char *path);
void destroyAnimAssetTable(AssetTable *tab);
int isAssetLoaded(const AssetTable *tab, uint32_t assetKey);
AssetTable *loadSpriteSheet(AssetDefTab *spriteSheetDef,
    SDL_Renderer *renderer);
void deleteAssetTable(AssetTable *tab);

#endif
