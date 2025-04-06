#ifndef K__SOUND_BGM_H
#define K__SOUND_BGM_H

#include "SDL_mixer.h"

#include "../asset/k_asset_registry.h"

struct k_sound_bgm {

    struct k_asset_registry_node registry_node;

    Mix_Music *music;
};

#endif
