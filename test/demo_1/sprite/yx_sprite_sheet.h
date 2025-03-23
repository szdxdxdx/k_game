#ifndef YX_SPRITE_SHEET_H
#define YX_SPRITE_SHEET_H

#include "k_game.h"
#include "k_json.h"

struct yx_sprite_sheet {
    struct k_image *image;
    float scale;
    struct k_json *config;
};

int yx_sprite_sheet_init(struct yx_sprite_sheet *sheet, const char *image_filepath, float scale, const char *config_filepath);

void yx_sprite_sheet_fini(struct yx_sprite_sheet *sheet);

struct k_sprite *yx_sprite_load_from_sheet(struct yx_sprite_sheet *sheet, const char *tag_name, float origin_x, float origin_y);

#endif
