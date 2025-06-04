
#define K_LOG_TAG "k_game:sound"
#include "k_log.h"

#include "k_game/core/k_sound.h"

#include "./k_sound.h"

/* region [bgm] */

void k_sound_bgm_loop(struct k_sound_bgm *sound, int loops) {

    if (NULL == sound) {
        k_log_error("sound is null");
        goto err;
    }

    if (loops <= 0) {
        /* TODO 暂停当前音乐？ */
        return;
    }

    if (0 != Mix_PlayMusic(sound->music, loops)) {
        k_log_error("SDL error: %s", Mix_GetError());
        goto err;
    }

    return;

err:;
}

/* endregion */

/* region [sfx] */

void k_sound_sfx_play(struct k_sound_sfx *sound) {
    k_sound_sfx_loop(sound, 1);
}

void k_sound_sfx_loop(struct k_sound_sfx *sound, int loops) {

    if (NULL == sound) {
        k_log_error("sound is null");
        goto err;
    }

    if (loops <= 0)
        return;

    /* 若指定 chunk 循环播放次数 `loops` 为 n 次，SDL 实际播放的是 n+1 次。这里需修正 `loops` */
    int loops_ = loops - 1;

    /* 让 SDL 在第一个可用的空闲音频通道上播放音频，若所有通道都被占用，则强制使用第 0 号通道 */
    if (-1 == Mix_PlayChannel(-1, sound->chunk, loops_)) {
        Mix_PlayChannel(0, sound->chunk, loops_);
    }

err:;
}

/* endregion */
