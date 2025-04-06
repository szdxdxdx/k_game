#ifndef K__SPRITE_H
#define K__SPRITE_H

#include "../asset/k_asset_registry.h"

struct k_image;

struct k_sprite_frame {

    struct k_image *image;

    int offset_x;
    int offset_y;

    int delay;
};

struct k_sprite {

    struct k_asset_registry_node registry_node;

    int sprite_w;
    int sprite_h;

    float origin_x;
    float origin_y;

    struct k_sprite_frame *frames;

    int frames_num;
};

#endif
