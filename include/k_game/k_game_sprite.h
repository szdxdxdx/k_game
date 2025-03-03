#ifndef K_GAME_SPRITE_H
#define K_GAME_SPRITE_H

#include "k_game_rect.h"

/**
 * \brief 精灵
 *
 * TODO docs
 */
struct k_sprite;

/* region [create_sprite] */

struct k_image;

/**
 * \brief 用于创建精灵的配置
 *
 * TODO docs
 */
struct k_sprite_frame_config {

    struct k_image *image;

    int offset_x, offset_y;

    int delay;
};

/**
 * \brief 用于创建精灵的配置
 *
 * TODO docs
 */
struct k_sprite_config {

    const char *sprite_name;

    int sprite_w, sprite_h;

    int origin_x, origin_y;

    struct k_sprite_frame_config *frames;

    int frames_num;
};

struct k_sprite *k_create_sprite(const struct k_sprite_config *config);

/* endregion */

/* region [sprite_get] */

struct k_sprite *k_get_sprite_by_name(const char *sprite_name);

int k_sprite_get_width(struct k_sprite *sprite);

int k_sprite_get_height(struct k_sprite *sprite);

/* endregion */

/* region [draw_sprite] */

int k_draw_sprite_frame(struct k_sprite *sprite, int x, int y, size_t frame_idx);

/* endregion */

#endif
