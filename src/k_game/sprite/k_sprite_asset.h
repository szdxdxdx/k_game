#ifndef K_GAME__SPRITE_ASSET_H
#define K_GAME__SPRITE_ASSET_H

#include "../asset/k_asset_registry.h"

struct k_sprite_frame {

    struct k_image *image;

    int offset_x, offset_y;

    int delay;
};

struct k_sprite {

    struct k_asset_registry_node registry_node;

    const char *sprite_name;

    int sprite_w, sprite_h;

    int origin_x, origin_y;

    struct k_sprite_frame *frames;

    int frames_num;
};

#endif
