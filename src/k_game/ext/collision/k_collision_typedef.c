#include "./_internal.h"

/* region [component_define] */

struct k_component_type *k__component_type_collision = NULL;

int k__define_component_collision_box(void) {

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct k_collision_box);
    entity_config.fn_init   = k__collision_box_init;
    entity_config.fn_fini   = k__collision_box_fini;

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct k_collision_manager);
    manager_config.fn_init   = k__collision_manager_init;
    manager_config.fn_fini   = k__collision_manager_fini;

    struct k_component_type *type = k_component_define(&manager_config, &entity_config);
    if (NULL == type)
        return -1;

    k__component_type_collision = type;
    return 0;
}

/* endregion */

/* region [room_add_component_manager] */

int k_room_add_collision_manager(void) {
    return k_room_add_component_manager(k__component_type_collision, NULL);;
}

/* endregion */

/* region [object_add_component] */

static struct k_collision_box *k__object_add_collision_box(struct k_object *object, enum k_collision_box_type box_type, const void *config) {

    struct k_collision_box_config box_config;
    box_config.type   = box_type;
    box_config.config = config;

    struct k_component *component = k_object_add_component(object, k__component_type_collision, &box_config);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

struct k_collision_box *k_object_add_collision_point(struct k_object *object, const struct k_collision_point_config *config) {
    if (NULL == config)    return NULL;
    if (NULL == config->x) return NULL;
    if (NULL == config->y) return NULL;
    return k__object_add_collision_box(object, K_COLLISION_POINT, config);
}

struct k_collision_box *k_object_add_collision_line(struct k_object *object, const struct k_collision_line_config *config) {

    if (NULL == config)
        return NULL;

    if (config->offset_x1 == config->offset_x2 || config->offset_y1 == config->offset_y2) {
        struct k_collision_rectangle_config rect_config;
        rect_config.group_id  = config->group_id;
        rect_config.x         = config->x;
        rect_config.y         = config->y;
        rect_config.offset_x1 = config->offset_x1;
        rect_config.offset_y1 = config->offset_y1;
        rect_config.offset_x2 = config->offset_x2;
        rect_config.offset_y2 = config->offset_y2;
        return k_object_add_collision_rectangle(object, &rect_config);
    }

    if (NULL == config->x || NULL == config->y)
        return NULL;

    return k__object_add_collision_box(object, K_COLLISION_LINE, config);
}

struct k_collision_box *k_object_add_collision_rectangle(struct k_object *object, const struct k_collision_rectangle_config *config) {

    if (NULL == config)
        return NULL;

    if (config->offset_x1 == config->offset_x2 && config->offset_y1 == config->offset_y2) {
        struct k_collision_point_config point_config;
        point_config.group_id = config->group_id;
        point_config.x        = config->x;
        point_config.y        = config->y;
        point_config.offset_x = config->offset_x1;
        point_config.offset_y = config->offset_y1;
        return k_object_add_collision_point(object, &point_config);
    }

    if (NULL == config->x || NULL == config->y)
        return NULL;

    return k__object_add_collision_box(object, K_COLLISION_RECTANGLE, config);
}

struct k_collision_box *k_object_add_collision_circle(struct k_object *object, const struct k_collision_circle_config *config) {

    if (NULL == config)
        return NULL;

    if (config->r <= 0.0f) {
        struct k_collision_point_config point_config;
        point_config.group_id = config->group_id;
        point_config.x        = config->x;
        point_config.y        = config->y;
        point_config.offset_x = config->offset_cx;
        point_config.offset_y = config->offset_cy;
        return k_object_add_collision_point(object, &point_config);
    }

    if (NULL == config->x || NULL == config->y)
        return NULL;

    return k__object_add_collision_box(object, K_COLLISION_CIRCLE, config);
}

void k_object_del_collision_box(struct k_collision_box *box) {
    if (NULL != box)
        k_object_del_component(box->component);
}

/* endregion */
