#ifndef CONTENT_H
#define CONTENT_H

#include "assets.h"

enum SpriteAssetKeys
{
    debugred,
};

enum AnimAssetKeys
{
    debuganim,
    debuganim2,
};

typedef enum SpriteAssetKeys SpriteAssetKeys;
typedef enum AnimAssetKeys AnimAssetKeys;

extern AssetDefTab spritesheet;
extern AssetDefTab animSpriteSheet;

#endif
