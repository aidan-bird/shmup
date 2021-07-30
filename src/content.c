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

/* XXX testing the animation system */
static const AssetDef animSpriteSheetDefs[] = {
    [debuganim] = {
        .key = debuganim,
        .path = "./assets/sprites/debuganim.png",
        .meta = {
            .anim = {
                .cellWidth = 32,
                .cellHeight = 32,
                .rows = 1,
                .cols = 4,
                .delays = (const unsigned short*[]) {
                    (unsigned short[]){
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
                .delays = (const unsigned short*[]) {
                    (unsigned short[]){
                        4, 4, 4, 4
                    },
                },
            },
        },
    },
};

/* XXX testing the animation system */
static const int animSpritesheetKeys[] = {
    debuganim,
    debuganim2,
};

/* XXX testing the animation system */
const AssetDefTab animSpriteSheet = {
    .label = "animation spritesheet",
    .count = LEN(animSpritesheetKeys),
    .keys = animSpritesheetKeys,
    .assetDefs = animSpriteSheetDefs,
};


