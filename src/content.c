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

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * validates tab.
 * prints all invalid entries in tab.
 * returns non-zero if tab is invalid.
 */
// int
// validateAssetDefTab(const AssetDef *tab, size_t n)
// {
//     int ret;
// 
//     ret = 0;
//     for (int i = 0; i < n; i++) {
//         if (!(tab[i].path)) {
//             ret = 1;
//             printf("%d: empty path", i);
//         }
//         if (tab[i].key < 0 || tab[i].key >= n) {
//             ret = 1;
//             printf("%d: invalid key", i);
//         }
//     }
//     if (!ret)
//         puts("AssetDefTab OK");
//     return ret;
// }

// const AssetDefTab sprites = spriteDefs;
// const size_t spriteCount = LEN(spriteDefs);

