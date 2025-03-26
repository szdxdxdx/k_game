#ifndef YX_SPRITE_SHEET_H
#define YX_SPRITE_SHEET_H

#include "k_game.h"

struct yx_sprite_sheet_sprite_config {

    struct k_sprite **get_sprite;

    const char *tag;

    float origin_x;
    float origin_y;
};

struct yx_sprite_sheet_config {

    const char *image_filepath;

    const char *config_filepath;

    float scale;

    struct yx_sprite_sheet_sprite_config *sprites;
};

#define YX_SPRITE_SHEET_CONFIG_INIT \
{ \
    .image_filepath  = NULL, \
    .config_filepath = NULL, \
    .scale           = 1.0f, \
    .sprites         = NULL, \
}

int yx_sprite_load_from_sheet(const struct yx_sprite_sheet_config *config);

#endif
