#include "k_game.h"

#include "../k_components_def.h"

struct k_sprite_renderer {

    float *x;
    float *y;

    struct k_sprite *sprite;

    uint64_t timer;

    size_t frame_idx;
};

static void renderer_draw(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    struct k_sprite *sprite = renderer->sprite;

    size_t frames_num = k_sprite_get_frames_num(sprite);

    renderer->timer += k_get_step_delta_ms();
    while (1) {
        int delay = k_sprite_get_frame_delay(sprite, renderer->frame_idx);
        if (renderer->timer < delay)
            break;

        renderer->timer -= delay;
        renderer->frame_idx += 1;
        renderer->frame_idx %= frames_num;
    }

    k_sprite_draw(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y));
}

static int renderer_init(struct k_component *component, void *params) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    struct k_sprite_renderer_config *config = params;

    renderer->timer     = 0;
    renderer->frame_idx = 0;
    renderer->sprite    = config->sprite;
    renderer->x         = config->x;
    renderer->y         = config->y;

    if (NULL == k_component_add_draw_callback(component, renderer_draw, config->z_index))
        return -1;

    return 0;
}

int k__component_def_sprite_renderer(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct k_sprite_renderer);
    config.fn_init = renderer_init;

    struct k_component_type *type = k_component_define(NULL, &config);
    if (NULL == type)
        return -1;

    k_component_type_set_name(type, "k/spr-rdr");
    return 0;
}
