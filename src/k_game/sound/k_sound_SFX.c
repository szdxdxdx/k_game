
#if 0
#include "k_game_alloc.h"

#include "./k_sound.h"

struct k_sound *k__sound_SFX_load(const char *filepath) {

    struct k_sound *sound = k_malloc(sizeof(struct k_sound));
    if (NULL == sound)
        return NULL;

    Mix_Chunk *chunk = Mix_LoadWAV(filepath);
    if (NULL == chunk) {
        k_free(sound);
        return NULL;
    }

    sound->sound_type = K_SOUND_SFX;
    sound->chunk = chunk;
    k__sound_registry_add(sound);
    return sound;
}

void k__sound_SFX_release(struct k_sound *sound) {

    k__sound_registry_del(sound);
    Mix_FreeChunk(sound->chunk);
    k_free(sound);
}

int k__sound_SFX_loop(struct k_sound *sound, int loops) {

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
#endif