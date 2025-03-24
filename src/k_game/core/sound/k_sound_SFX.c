#include "_internal.h"

/* region [sound_registry] */

static struct k_asset_registry SFX_registry;

int k__init_sound_SFX_registry(void) {
    return k__asset_registry_init(&SFX_registry);
}

static void release_asset(struct k_asset_registry_node *registry_node) {
    struct k_sound_SFX *sound = (struct k_sound_SFX *)registry_node;
    k_sound_SFX_release(sound);
}

void k__cleanup_sound_SFX_registry(void) {
    k__asset_registry_cleanup(&SFX_registry, release_asset);
}

struct k_sound_SFX *k_find_sound_SFX(const char *SFX_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&SFX_registry, SFX_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound_SFX *sound = container_of(registry_node, struct k_sound_SFX, registry_node);
    return sound;
}

/* endregion */

/* region [sound_load] */

struct k_sound_SFX *k_sound_SFX_load(const char *filepath) {

    struct k_sound_SFX *sound = k_malloc(sizeof(struct k_sound_SFX));
    if (NULL == sound)
        return NULL;

    Mix_Chunk *chunk = Mix_LoadWAV(filepath);
    if (NULL == chunk) {
        k_free(sound);
        return NULL;
    }

    sound->chunk = chunk;
    k__asset_registry_add(&SFX_registry, &sound->registry_node);
    return sound;
}

void k_sound_SFX_release(struct k_sound_SFX *sound) {

    if (NULL == sound)
        return;

    k__asset_registry_del(&sound->registry_node);
    Mix_FreeChunk(sound->chunk);
    k_free(sound);
}

/* endregion */

/* region [sound_play] */

void k_sound_SFX_play(struct k_sound_SFX *sound) {

    if (NULL == sound)
        return;

    /* 让 SDL 在第一个可用的空闲音频通道上播放音频，
     * 若所有通道都被占用，则强制使用第 0 号通道
     */
    if (-1 == Mix_PlayChannel(-1, sound->chunk, 0)) {
        Mix_PlayChannel(0, sound->chunk, 0);
    }
}

void k_sound_SFX_loop(struct k_sound_SFX *sound, int loops) {

    if (NULL == sound)
        return;

    if (loops <= 0)
        return;

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
}

/* endregion */
