#include "k_log.h"

#include "./k_sound.h"

int k_sound_play(struct k_sound *sound) {
    return k_sound_loop(sound, 1);
}

int k_sound_loop(struct k_sound *sound, int loops) {

    if (NULL == sound) {
        k_log_error("Failed to play sound. Sound is NULL");
        return -1;
    }

    if (loops <= 0)
        return 0;

    switch (sound->sound_type) {

        case K_SOUND_BGM: {
            if (0 != Mix_PlayMusic(sound->music, loops)) {
                k_log_error("Mix_PlayMusic(): %s", Mix_GetError());
                return -1;
            } else {
                return 0;
            }
        }

        case K_SOUND_SFX: {
            /* 若指定 chunk 循环播放次数 `loops` 为 n 次，
             * SDL 实际播放的是 n+1 次。这里需修正 `loops`。
             */
            int loops_ = loops - 1;

            /* 让 SDL 在第一个可用的空闲音频通道上播放音频，
             * 若所有通道都被占用，则强制使用第 0 号通道
             */
            if (-1 == Mix_PlayChannel(-1, sound->chunk, loops_)) {
                Mix_PlayChannel(0, sound->chunk, loops_);
            }

            return 0;
        }

        default:
            return -1;
    }
}
