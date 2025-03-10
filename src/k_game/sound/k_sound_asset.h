#ifndef K_GAME__SOUND_ASSET_H
#define K_GAME__SOUND_ASSET_H

#include "SDL_mixer.h"

#include "../asset/k_asset_registry.h"

struct k_sound_BGM {

    Mix_Music *sound;
};

struct k_sound_SFX {

    Mix_Chunk *sound;
};

struct k_sound {

    Mix_Music *sound;
};

#endif
