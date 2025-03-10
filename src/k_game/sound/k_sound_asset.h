#ifndef K_GAME__SOUND_ASSET_H
#define K_GAME__SOUND_ASSET_H

#include "SDL_mixer.h"

#include "../asset/k_asset_registry.h"

struct k_sound {

    struct k_asset_registry_node registry_node;

    enum k_sound_type sound_type;

    union {
        Mix_Music *music;
        Mix_Chunk *chunk;
    };
};

#endif
