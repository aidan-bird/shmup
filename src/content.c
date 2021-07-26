#include "./utils.h"
#include "./content.h"
#include "./assets.h"

static const AssetDef spritesheetDefs[] = {
    [debugred] = {
        .key = debugred,
        .path = "./assets/sprites/debugred.png",
        .meta = {
            .sprite = {
                .cellWidth = 32,
                .cellHeight = 32,
                .rows = 1,
                .cols = 1,
            },
        },
    },
};

static const int spritesheetKeys[] = {
    debugred,
};

const AssetDefTab spritesheet = {
    .label = "spritesheet",
    .count = LEN(spritesheetKeys),
    .keys = spritesheetKeys,
    .assetDefs = spritesheetDefs,
};

