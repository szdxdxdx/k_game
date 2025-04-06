#include "_internal.h"

/* region [sound_registry] */

static struct k_asset_registry sfx_registry;

int k__init_sound_sfx_registry(void) {
    return k__asset_registry_init(&sfx_registry);
}

static void release_asset(struct k_asset_registry_node *registry_node) {
    struct k_sound_sfx *sound = container_of(registry_node, struct k_sound_sfx, registry_node);
    k_sound_sfx_release(sound);
}

void k__cleanup_sound_sfx_registry(void) {
    k__asset_registry_cleanup(&sfx_registry, release_asset);
}

struct k_sound_sfx *k_sound_sfx_find(const char *sfx_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&sfx_registry, sfx_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound_sfx *sound = container_of(registry_node, struct k_sound_sfx, registry_node);
    return sound;
}

/* endregion */

/* region [sound_load] */

struct k_sound_sfx *k_sound_sfx_load(const char *filepath) {

    if (NULL == filepath || '\0' == filepath[0])
        return NULL;

    struct k_sound_sfx *sound = k_malloc(sizeof(struct k_sound_sfx));
    if (NULL == sound)
        return NULL;

    Mix_Chunk *chunk = Mix_LoadWAV(filepath);
    if (NULL == chunk) {
        k_free(sound);
        return NULL;
    }

    sound->chunk = chunk;
    k__asset_registry_add(&sfx_registry, &sound->registry_node);
    return sound;
}

void k_sound_sfx_release(struct k_sound_sfx *sound) {

    if (NULL == sound)
        return;

    k__asset_registry_del(&sound->registry_node);
    Mix_FreeChunk(sound->chunk);
    k_free(sound);
}

/* endregion */

/* region [sound_play] */

void k_sound_sfx_play(struct k_sound_sfx *sound) {

    if (NULL == sound)
        return;

    /* 让 SDL 在第一个可用的空闲音频通道上播放音频，
     * 若所有通道都被占用，则强制使用第 0 号通道
     */
    if (-1 == Mix_PlayChannel(-1, sound->chunk, 0)) {
        Mix_PlayChannel(0, sound->chunk, 0);
    }
}

void k_sound_sfx_loop(struct k_sound_sfx *sound, int loops) {

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
