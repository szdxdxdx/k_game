#include <stddef.h>

#include "k_game/core/k_sound.h"

#include "sound/yx_sound.h"

struct k_sound_bgm *yx_bgm_music;
struct k_sound_sfx *yx_sfx_fire;
struct k_sound_sfx *yx_sfx_click;
struct k_sound_sfx *yx_sfx_apple_break[3];
struct k_sound_sfx *yx_sfx_banner;

int yx_sound_load_on_game_start(void) {

    if (NULL == (yx_bgm_music = k_sound_bgm_load("./demo_1/sound/bgm2.mp3"))) return -1;
    if (NULL == (yx_sfx_fire = k_sound_sfx_load("./demo_1/sound/fire2.wav"))) return -1;
    if (NULL == (yx_sfx_click = k_sound_sfx_load("./demo_1/sound/click.wav"))) return -1;
    if (NULL == (yx_sfx_apple_break[0] = k_sound_sfx_load("./demo_1/sound/apple_break_1.mp3"))) return -1;
    if (NULL == (yx_sfx_apple_break[1] = k_sound_sfx_load("./demo_1/sound/apple_break_2.mp3"))) return -1;
    if (NULL == (yx_sfx_apple_break[2] = k_sound_sfx_load("./demo_1/sound/apple_break_3.mp3"))) return -1;
    if (NULL == (yx_sfx_banner = k_sound_sfx_load("./demo_1/sound/banner.mp3"))) return -1;

    return 0;
}
