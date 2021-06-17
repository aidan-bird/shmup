#include "./utils.h"
#include "./content.h"

static const AssetDef spriteDefs[] = {
    {
        .key = debugred,
        .path = "./assets/sprites/debugred.png",
    },
};

const AssetDefTab sprites = spriteDefs;
const size_t spriteCount = LEN(spriteDefs);

