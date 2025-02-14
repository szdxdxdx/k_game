#ifndef K_GAME_SPRITE_H
#define K_GAME_SPRITE_H

#include "rect.h"

struct k_image;

struct k_sprite;

struct k_sprite_config {

    const char *sprite_name;

    int sprite_w, sprite_h;

    int origin_x, origin_y;

    struct k_image *image;

    int frames_num;

    struct k_int_point *frames;

    float *frame_delays;
};

struct k_sprite *k_create_sprite(const struct k_sprite_config *config);

struct k_sprite *k_get_sprite_by_name(const char *sprite_name);

int k_sprite_get_width(struct k_sprite *sprite);

int k_sprite_get_height(struct k_sprite *sprite);

#endif
