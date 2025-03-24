#ifndef K_SPRITE_PUBLIC_H
#define K_SPRITE_PUBLIC_H

#include "../asset/k_asset_registry.h"

struct k_image;

int k__sprite_registry_init(void);

void k__sprite_registry_cleanup(void);

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
