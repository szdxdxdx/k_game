#ifndef K_GAME__SPRITE_H
#define K_GAME__SPRITE_H

#include "k_game_sprite.h"

#include "../asset/asset_registry.h"

/* region [sprite_registry] */

int k__sprite_registry_init(void);

void k__sprite_registry_cleanup(void);

int k__sprite_registry_add(struct k_sprite *sprite, const char *sprite_name);

void k__sprite_registry_del(struct k_sprite *sprite);

/* endregion */

/* region [sprite] */

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

void k__sprite_destroy(struct k_sprite *sprite);

/* endregion */

/* region [sprite_draw] */

int k__sprite_draw_frame(struct k_sprite *sprite, int dst_x, int dst_y, size_t frame_idx);

/* endregion */

#endif
