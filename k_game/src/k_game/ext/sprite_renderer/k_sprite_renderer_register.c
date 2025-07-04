#include <limits.h>

#include "./k_sprite_renderer_internal.h"

/* region [renderer_init] */

static int k__sprite_renderer_on_create(struct k_component *component, void *param) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);
    const struct k_sprite_renderer_config *config = param;

    renderer->component = component;

    renderer->z_group = config->z_group;
    renderer->z_layer = config->z_layer;
    renderer->cb_draw_sprite = k_component_add_draw_callback(
        component,
        k__sprite_renderer_on_draw,
        renderer->z_group,
        renderer->z_layer
    );
    if (NULL == renderer->cb_draw_sprite)
        return -1;

    renderer->cb_debug_draw = NULL;

    renderer->x = config->x;
    renderer->y = config->y;

    renderer->sprite = NULL;

    renderer->loop_count = INT_MAX;
    renderer->fn_loop_callback = NULL;

    renderer->frame_idx = 0;
    renderer->timer = 0.0f;
    renderer->speed = 1.0f;

    renderer->scaled_w = 0.0f;
    renderer->scaled_h = 0.0f;
    renderer->scale_x  = 1.0f;
    renderer->scale_y  = 1.0f;
    renderer->angle    = 0.0f;
    renderer->transform_flags = transform_none;

    if (NULL != config->sprite) {
        k_sprite_renderer_set_sprite(renderer, config->sprite);
    }

    return 0;
}

/* endregion */

/* region [component_type_register] */

static struct k_component_type *k__component_type_sprite_renderer = NULL;

int k__component_type_register_sprite_renderer(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct k_sprite_renderer);
    config.on_create = k__sprite_renderer_on_create;

    struct k_component_type *type = k_component_type_register(NULL, &config);
    if (NULL == type)
        return -1;

    k__component_type_sprite_renderer = type;
    return 0;
}

/* endregion */

/* region [object_add_component] */

struct k_sprite_renderer *k_object_add_sprite_renderer(struct k_object *object, const struct k_sprite_renderer_config *config) {
    if (NULL == config)    return NULL;
    if (NULL == config->x) return NULL;
    if (NULL == config->y) return NULL;

    struct k_component *component = k_object_add_component(object, k__component_type_sprite_renderer, (void*)config);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void k_object_del_sprite_renderer(struct k_sprite_renderer *renderer) {

    if (NULL == renderer)
        return;

    k_object_del_component(renderer->component);
}

/* endregion */
