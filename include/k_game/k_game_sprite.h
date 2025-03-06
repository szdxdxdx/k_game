#ifndef K_GAME_SPRITE_H
#define K_GAME_SPRITE_H

#include <stddef.h>

#include "k_game_rect.h"

/**
 * \brief 精灵
 *
 * TODO docs
 */
struct k_sprite;

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

struct k_sprite *k_sprite_create(const struct k_sprite_config *config);

struct k_sprite *k_sprite_find(const char *sprite_name);

int k_sprite_get_width(struct k_sprite *sprite);

int k_sprite_get_height(struct k_sprite *sprite);

int k_sprite_draw_frame(struct k_sprite *sprite, int x, int y, size_t frame_idx);

#endif
