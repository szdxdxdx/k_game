#include <stddef.h>

#include "k_game/core/k_sound.h"

#include "sound/yx_sound.h"

struct k_sound_bgm *yx_bgm_music = NULL;

int yx_sound_load_on_game_start(void) {

    if (NULL == (yx_bgm_music = k_sound_bgm_load("./demo_1/sound/bgm.wav")))
        return -1;

    return 0;
}
