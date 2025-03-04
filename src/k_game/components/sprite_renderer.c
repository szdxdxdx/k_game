#include "k_game/sprite_asset.h"
#include "k_game/sprite_draw.h"
#include "k_game_time.h"
#include "k_game_component.h"
#include "k_game_components/sprite_renderer.h"

struct k_sprite_renderer {

    uint64_t timer;

    size_t frame_idx;

    struct k_sprite *sprite;

    float *x, *y;
};

static void sprite_renderer_draw(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    uint64_t step_delta_ms = k_get_step_delta_ms();

    struct k_sprite *sprite = renderer->sprite;
    renderer->timer += step_delta_ms;
    while (1) {
        int delay = sprite->frames[renderer->frame_idx].delay;
        if (renderer->timer < delay)
            break;

        renderer->timer -= delay;
        renderer->frame_idx += 1;
        renderer->frame_idx %= sprite->frames_num;
    }

    float x = *(renderer->x);
    float y = *(renderer->y);
    k__sprite_draw_frame(sprite, (int)x, (int)y, renderer->frame_idx);
}

static int sprite_renderer_init(struct k_component *component, void *params) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    struct k_sprite_renderer_config *config = params;

    renderer->timer     = 0;
    renderer->frame_idx = 0;
    renderer->sprite    = config->sprite;
    renderer->x         = config->x;
    renderer->y         = config->y;

    if (NULL == k_component_add_draw_callback(component, sprite_renderer_draw, config->z_index))
        return -1;

    return 0;
}

int k__component_def_sprite_renderer(void) {

    struct k_component_type_config config = K_COMPONENT_TYPE_CONFIG_INIT;
    config.type_name = "k/spr-rdr";
    config.data_size = sizeof(struct k_sprite_renderer);
    config.fn_init = sprite_renderer_init;

    struct k_component_type *type = k_component_define(&config);

    return NULL == type ? -1 : 0;
}
