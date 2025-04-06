#ifndef K__SOUND_SFX_H
#define K__SOUND_SFX_H

#include "SDL_mixer.h"

#include "../asset/k_asset_registry.h"

struct k_sound_sfx {

    struct k_asset_registry_node registry_node;

    Mix_Chunk *chunk;
};

#endif
