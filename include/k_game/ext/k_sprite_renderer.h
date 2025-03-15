#ifndef K_GAME_COMPONENT_SPRITE_RENDERER_H
#define K_GAME_COMPONENT_SPRITE_RENDERER_H

struct k_sprite;

struct k_sprite_renderer_config {

    float *x;
    float *y;

    struct k_sprite *sprite;

    int z_index;
};

struct k_sprite_renderer;

struct k_sprite_renderer *k_object_add_sprite_renderer(struct k_object *object, const struct k_sprite_renderer_config *config);

void k_object_del_sprite_renderer(struct k_sprite_renderer *renderer);

void k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite);
struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer);

void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed);
void k_sprite_renderer_add_speed(struct k_sprite_renderer *renderer, float speed_add);
float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer);

int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_index);

/* region [transform] */

void k_sprite_renderer_set_scale_w(struct k_sprite_renderer *renderer, int scaled_w);
void k_sprite_renderer_add_scale_w(struct k_sprite_renderer *renderer, int w_add);
int k_sprite_renderer_get_scale_w(struct k_sprite_renderer *renderer);

void k_sprite_renderer_set_scale_h(struct k_sprite_renderer *renderer, int scaled_h);
void k_sprite_renderer_add_scale_h(struct k_sprite_renderer *renderer, int h_add);
int k_sprite_renderer_get_scale_h(struct k_sprite_renderer *renderer);

void k_sprite_renderer_set_rotation(struct k_sprite_renderer *renderer, float angle);
void k_sprite_renderer_add_rotation(struct k_sprite_renderer *renderer, float angle_add);
float k_sprite_renderer_get_rotation(struct k_sprite_renderer *renderer);

void k_sprite_renderer_flip_x(struct k_sprite_renderer *renderer);
int k_sprite_renderer_is_flipped_x(struct k_sprite_renderer *renderer);

void k_sprite_renderer_flip_y(struct k_sprite_renderer *renderer);
int k_sprite_renderer_is_flipped_y(struct k_sprite_renderer *renderer);

void k_sprite_renderer_clear_transforms(struct k_sprite_renderer *renderer);

/* endregion */

#endif
