
#ifndef YX_SOUND_H
#define YX_SOUND_H

#include "k_game/core/k_sound.h"

extern struct k_sound_bgm *yx_bgm_music;
extern struct k_sound_sfx *yx_sfx_fire;
extern struct k_sound_sfx *yx_sfx_click;

extern struct k_sound_sfx *yx_sfx_apple_break[3];

extern struct k_sound_sfx *yx_sfx_banner;

int yx_sound_load_on_game_start(void);

#endif
