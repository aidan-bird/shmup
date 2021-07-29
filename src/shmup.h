#ifndef SHMUP_H
#define SHMUP_H

#include "./assets.h"

typedef enum AssetDefTabKey AssetDefTabKey;

enum AssetDefTabKey
{
    debugSpriteSheet,
}

extern const AssetDefTabKey *listOfAssetDefTabKeys;

extern AssetDefTab assetDefTables;

#endif
