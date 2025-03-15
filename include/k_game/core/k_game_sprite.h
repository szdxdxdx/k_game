#ifndef K_GAME_SPRITE_H
#define K_GAME_SPRITE_H

#include <stddef.h>

#include "k_game_rect.h"

struct k_image;

/**
 * \brief 精灵
 *
 * TODO docs
 */
struct k_sprite;

/* region [sprite_create] */

/**
 * \brief 用于创建精灵的配置
 *
 * TODO docs
 */
struct k_sprite_frame_config {

    struct k_image *image;

    int offset_x;
    int offset_y;

    int delay;
};

/**
 * \brief 用于创建精灵的配置
 *
 * TODO docs
 */
struct k_sprite_config {

    int sprite_w;
    int sprite_h;

    float origin_x;
    float origin_y;

    struct k_sprite_frame_config *frames;

    int frames_num;
};

struct k_sprite *k_sprite_create(const struct k_sprite_config *config);

int k_sprite_set_name(struct k_sprite *sprite, const char *sprite_name);

struct k_sprite *k_sprite_find(const char *sprite_name);

/* endregion */

/* region [sprite_get] */

int k_sprite_get_width(struct k_sprite *sprite);

int k_sprite_get_height(struct k_sprite *sprite);

int k_sprite_get_frames_num(struct k_sprite *sprite);

int k_sprite_get_frame_delay(struct k_sprite *sprite, size_t frame_idx);

/* endregion */

/* region [sprite_draw] */

int k_sprite_draw_frame(struct k_sprite *sprite, size_t frame_idx, float x, float y);

/* endregion */

#endif
