#ifndef K_SOUND_INTERNAL_H
#define K_SOUND_INTERNAL_H

#include "SDL_mixer.h"

#include "k_game.h"

#include "../asset/k_asset_registry.h"

#include "./_public.h"

struct k_sound_BGM {

    struct k_asset_registry_node registry_node;

    Mix_Music *music;
};

struct k_sound_SFX {

    struct k_asset_registry_node registry_node;

    Mix_Chunk *chunk;
};

#endif
