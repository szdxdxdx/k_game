#ifndef K_GAME__SOUND_ASSET_H
#define K_GAME__SOUND_ASSET_H

#include "SDL_mixer.h"

#include "../asset/k_asset_registry.h"

struct k_sound {

    Mix_Music *music;
};

#endif
