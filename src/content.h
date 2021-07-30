#ifndef CONTENT_H
#define CONTENT_H

#include <stddef.h>

#include "assets.h"

typedef enum SpriteAssetKeys SpriteAssetKeys;

enum SpriteAssetKeys
{
    debugred,
};

enum AnimAssetKeys
{
    debuganim,
    debuganim2,
};

const extern AssetDefTab spritesheet;

const extern AssetDefTab animSpriteSheet;

#endif
