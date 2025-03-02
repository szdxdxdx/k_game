#ifndef K_GAME__COMPONENT_SPRITE_RENDERER_H
#define K_GAME__COMPONENT_SPRITE_RENDERER_H

struct k_sprite;
struct k_object;

struct k_sprite_renderer;

struct k_sprite_renderer_config {

    struct k_sprite *sprite;

    float speed;

    int depth;

    void (*fn_draw)(struct k_sprite_renderer *renderer, struct k_object *object);
};

int k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite);

int k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed);

int k_sprite_renderer_set_depth(struct k_sprite_renderer *renderer, int depth);

int k_sprite_renderer_set_x(struct k_sprite_renderer *renderer, int x);

int k_sprite_renderer_set_y(struct k_sprite_renderer *renderer, int y);

#endif
