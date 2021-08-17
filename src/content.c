#include "./utils.h"
#include "./content.h"
#include "./assets.h"

static AssetDef spritesheetDefs[] = {
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

static size_t spritesheetKeys[] = {
    debugred,
};

AssetDefTab spritesheet = {
    .label = "spritesheet",
    .count = LEN(spritesheetKeys),
    .keys = spritesheetKeys,
    .assetDefs = spritesheetDefs,
};

/* XXX testing the animation system */
static AssetDef animSpriteSheetDefs[] = {
    [debuganim] = {
        .key = debuganim,
        .path = "./assets/sprites/debuganim.png",
        .meta = {
            .anim = {
                .cellWidth = 32,
                .cellHeight = 32,
                .rows = 1,
                .cols = 4,
                .delays = (const uint16_t*[]) {
                    (uint16_t[]){
                        2, 2, 2, 2
                    },
                },
            },
        },
    },
    [debuganim2] = {
        .key = debuganim2,
        .path = "./assets/sprites/debuganim2.png",
        .meta = {
            .anim = {
                .cellWidth = 32,
                .cellHeight = 32,
                .rows = 1,
                .cols = 4,
                .delays = (const uint16_t*[]) {
                    (uint16_t[]){
                        4, 4, 4, 4
                    },
                },
            },
        },
    },
};

/* XXX testing the animation system */
static size_t animSpritesheetKeys[] = {
    debuganim,
    debuganim2,
};

/* XXX testing the animation system */
AssetDefTab animSpriteSheet = {
    .label = "animation spritesheet",
    .count = LEN(animSpritesheetKeys),
    .keys = animSpritesheetKeys,
    .assetDefs = animSpriteSheetDefs,
};


