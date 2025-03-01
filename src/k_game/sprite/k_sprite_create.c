#include "k_log.h"

#include "k_game/alloc.h"
#include "../image/k_image_load.h"

#include "k_game/sprite.h"
#include "./k_sprite_create.h"

static int check_config(const struct k_sprite_config *config) {

    const char *err_msg;

#define check_config_assert(cond) \
    if ( ! (cond)) { err_msg = "assert( " #cond " )"; goto err; }

    check_config_assert(NULL != config);
    check_config_assert(0 != config->frames_num);

    size_t i = 0;
    for (; i < config->frames_num; i++)
        check_config_assert(0 < config->frames[i].delay);

    return 0;

err:
    k_log_error("Invalid sprite config: %s", err_msg);
    return -1;
}

struct k_sprite *k_create_sprite(const struct k_sprite_config *config) {

    if (0 != check_config(config))
        return NULL;

    struct k_sprite *sprite = k_malloc(sizeof(struct k_sprite));
    if (NULL == sprite)
        return NULL;

    struct k_sprite_frame *frames = k_malloc(config->frames_num * sizeof(struct k_sprite_frame));
    if (NULL == frames) {
        k_free(frames);
        return NULL;
    }

    size_t i = 0;
    for (; i < config->frames_num; i++) {
        frames[i].texture  = config->frames[i].image->texture;
        frames[i].offset_x = config->frames[i].offset_x;
        frames[i].offset_y = config->frames[i].offset_y;
        frames[i].delay    = config->frames[i].delay;
    }

    // TODO sprite->sprite_name
    sprite->sprite_w = config->sprite_w;
    sprite->sprite_h = config->sprite_h;
    sprite->origin_x = config->origin_x;
    sprite->origin_y = config->origin_y;
    sprite->frames = frames;

    return sprite;
}
