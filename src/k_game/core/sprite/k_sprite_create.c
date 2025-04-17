#include "k_log.h"
#include "k_seq_step.h"

#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_sprite.h"

#include "./k_sprite.h"
#include "./k_sprite_registry.h"

static int check_config(const struct k_sprite_config *config) {

    const char *err_msg;

#define check_config_assert(cond) \
    do { if ( ! (cond)) { err_msg = "assert( " #cond " )"; goto err; }} while(0)

    check_config_assert(NULL != config);
    check_config_assert(0 < config->frames_num);

    size_t i = 0;
    for (; i < config->frames_num; i++) {
        check_config_assert(NULL != config->frames[i].image);
        check_config_assert(0 < config->frames[i].delay);
    }

    if (1024 <= config->frames_num) {
        err_msg = "Too many frames";
        goto err;
    }

    return 0;

err:
    k_log_error("Invalid sprite config: %s", err_msg);
    return -1;
}

struct k_sprite *k_sprite_create(const struct k_sprite_config *config) {

    if (0 != check_config(config))
        goto err;

    /* sprite 结构体和精灵帧 frames 共处同一个大内存块 */
    size_t frames_size = config->frames_num * sizeof(struct k_sprite_frame);
    struct k_sprite *sprite = k__mem_alloc(sizeof(struct k_sprite) + frames_size);
    if (NULL == sprite)
        goto err;

    struct k_sprite_frame *frames = (struct k_sprite_frame *)&(sprite[1]);
    size_t i = 0;
    for (; i < config->frames_num; i++) {
        frames[i].image    = config->frames[i].image;
        frames[i].offset_x = config->frames[i].offset_x;
        frames[i].offset_y = config->frames[i].offset_y;
        frames[i].delay    = config->frames[i].delay;
    }

    sprite->sprite_w   = config->sprite_w;
    sprite->sprite_h   = config->sprite_h;
    sprite->origin_x   = config->origin_x;
    sprite->origin_y   = config->origin_y;
    sprite->frames     = frames;
    sprite->frames_num = config->frames_num;

    k__sprite_registry_add(sprite);
    return sprite;

err:
    k_log_error("Failed to create sprite");
    return NULL;
}

void k_sprite_destroy(struct k_sprite *sprite) {

    if (NULL == sprite)
        return;

    k__sprite_registry_del(sprite);
    k__mem_free(sprite);
}
