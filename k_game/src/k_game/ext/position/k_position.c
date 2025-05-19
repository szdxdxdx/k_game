#include "k_list.h"

#include "k_game/core/k_component.h"

#include "k_game/ext/k_position.h"
#include "./k_position_type_register.h"

struct k_position {

    struct k_list_node sibling_link;

    struct k_position *parent;

    struct k_list child_list;

    struct k_component *component;

    float local_x;
    float local_y;

    float *world_x;
    float *world_y;
};

/* region [world] */

static struct k_position world;

static void world_init(void) {

    k_list_node_loop(&world.sibling_link);
    world.parent = NULL;

    k_list_init(&world.child_list);

    world.component = NULL;

    world.local_x = 0;
    world.local_y = 0;

    static float x = 0;
    static float y = 0;
    world.world_x = &x;
    world.world_y = &y;
}

/* endregion */

/* region [position_update] */

static void position_update(struct k_position *position) {

    *(position->world_x) = *(position->parent->world_x) + position->local_x;
    *(position->world_y) = *(position->parent->world_y) + position->local_y;

    struct k_position *child;
    struct k_list *list = &position->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        child = container_of(iter, struct k_position, sibling_link);

        position_update(child);
    }
}

/* endregion */

/* region [position_set] */

void k_position_set_local_position(struct k_position *position, float local_x, float local_y) {

    position->local_x = local_x;
    position->local_y = local_y;

    position_update(position);
}

void k_position_set_world_position(struct k_position *position, float world_x, float world_y) {

    position->local_x = world_x - *(position->parent->world_x);
    position->local_y = world_y - *(position->parent->world_y);

    position_update(position);
}

/* endregion */

/* region [position_on_create] */

static int k__position_on_create(struct k_component *component, void *param) {
    struct k_position *position = k_component_get_data(component);
    const struct k_position_config *config = param;

    struct k_position *parent = config->parent;
    if (NULL == parent)
        parent = &world;

    position->parent = parent;
    k_list_insert_tail(&parent->child_list, &position->sibling_link);

    k_list_init(&position->child_list);

    position->local_x = config->local_x;
    position->local_y = config->local_y;
    position->world_x = config->world_x;
    position->world_y = config->world_y;

    *(position->world_x) = *(position->parent->world_x) + position->local_x;
    *(position->world_y) = *(position->parent->world_y) + position->local_y;

    return 0;
}

static void k__position_on_destroy(struct k_component *component) {
    struct k_position *position = k_component_get_data(component);

    struct k_position *parent = position->parent;

    k_list_remove(&position->sibling_link);

    struct k_position *child;
    struct k_list *list = &position->child_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        child = container_of(iter, struct k_position, sibling_link);

        k_list_insert_tail(&parent->child_list, &child->sibling_link);
        child->parent = parent;
        child->local_x += position->local_x;
        child->local_y += position->local_y;
    }
}

/* endregion */

/* region [component_type_register] */

static struct k_component_type *k__position_component_type = NULL;

int k__component_type_register_position(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size  = sizeof(struct k_position);
    config.on_create  = k__position_on_create;
    config.on_destroy = k__position_on_destroy;

    struct k_component_type *type = k_component_type_register(NULL, &config);
    if (NULL == type)
        return -1;

    world_init();

    k__position_component_type = type;
    return 0;
}

/* endregion */

/* region [object_add_component] */

struct k_position *k_object_add_position(struct k_object *object, const struct k_position_config *config) {
    if (NULL == config)          return NULL;
    if (NULL == config->world_x) return NULL;
    if (NULL == config->world_y) return NULL;

    struct k_component *component = k_object_add_component(object, k__position_component_type, (void*)config);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void k_object_del_position(struct k_position *position) {

    if (NULL == position)
        return;

    k_object_del_component(position->component);
}

/* endregion */
