#ifndef K_GAME_COMPONENT_SPRITE_RENDERER_H
#define K_GAME_COMPONENT_SPRITE_RENDERER_H

struct k_sprite;

struct k_sprite_renderer_config {

    struct k_sprite *sprite;

    int z_index;

    float *x, *y;
};

#endif
