#ifndef CONTENT_H
#define CONTENT_H

#include <stddef.h>

#include "assets.h"

enum SpriteAssetKeys
{
    debugred,
};

int validateAssetDefTab(const AssetDef *tab, size_t n);

const extern AssetDefTab sprites;
const extern size_t spriteCount;

#endif
