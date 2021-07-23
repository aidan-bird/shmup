#ifndef CONTENT_H
#define CONTENT_H

#include <stddef.h>

#include "assets.h"

typedef enum SpriteAssetKeys SpriteAssetKeys;

enum SpriteAssetKeys
{
    debugred,
};

const extern AssetDefTab spritesheet;

// int validateAssetDefTab(const AssetDef *tab, size_t n);

#endif
