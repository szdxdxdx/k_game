#include "k_game_time.h"
#include "k_game_component.h"

#include "../sprite/k_sprite.h"

#include "k_game_components/k_sprite_renderer.h"
#include "./k_components_def.h"

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

    renderer->timer += k_get_step_delta_ms();
    while (1) {
        int delay = sprite->frames[renderer->frame_idx].delay;
        if (renderer->timer < delay)
            break;

        renderer->timer -= delay;
        renderer->frame_idx += 1;
        renderer->frame_idx %= sprite->frames_num;
    }

    k__sprite_draw_frame(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y));
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

    struct k_component_type_config config = K_COMPONENT_TYPE_CONFIG_INIT;
    config.type_name = "k/spr-rdr";
    config.data_size = sizeof(struct k_sprite_renderer);
    config.fn_init = renderer_init;

    return NULL != k_component_define(&config) ? 0 : -1;
}
