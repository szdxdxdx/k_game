#include <limits.h>

#include "./_internal.h"

/* region [renderer_init] */

void k__sprite_renderer_reset(struct k_sprite_renderer *renderer) {

    renderer->loop = INT_MAX;
    renderer->fn_loop_callback = NULL;

    renderer->frame_idx = 0;
    renderer->timer = 0;
    renderer->speed = 1;

    k_sprite_renderer_reset_transforms(renderer);

    k_sprite_renderer_set_debug(renderer, renderer->debug);
}

static int sprite_renderer_init(struct k_component *component, void *params) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    struct k_sprite_renderer_config *config = params;

    renderer->component = component;

    renderer->z_group = config->z_group;
    renderer->z_layer = config->z_layer;
    renderer->callback_draw_sprite = NULL;

    renderer->debug = 0;
    renderer->callback_draw_rect = NULL;

    renderer->position = config->position;

    renderer->sprite = NULL;

    if (NULL == config->sprite) {
        k__sprite_renderer_reset(renderer);
    }
    else {
        if (0 != k_sprite_renderer_set_sprite(renderer, config->sprite))
            return -1;
    }

    return 0;
}

/* endregion */

/* region [component_define] */

static struct k_component_type *k__component_type_sprite_renderer = NULL;

int k__component_def_sprite_renderer(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct k_sprite_renderer);
    config.fn_init = sprite_renderer_init;

    k__component_type_sprite_renderer = k_component_define(NULL, &config);
    if (NULL == k__component_type_sprite_renderer)
        return -1;

    return 0;
}

/* endregion */

/* region [object_add_sprite_renderer] */

struct k_sprite_renderer *k_object_add_sprite_renderer(struct k_object *object, const struct k_sprite_renderer_config *config) {

    if (NULL == config)
        return NULL;
    if (NULL == config->position)
        return NULL;

    struct k_component *component = k_object_add_component(object, k__component_type_sprite_renderer, (void*)config);
    if (NULL == component)
        return NULL;
    else
        return k_component_get_data(component);
}

void k_object_del_sprite_renderer(struct k_sprite_renderer *renderer) {
    if (NULL != renderer)
        k_object_del_component(renderer->component);
}

/* endregion */
