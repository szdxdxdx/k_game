#include "_internal.h"

/* region [world] */

static struct k_position world;

static void world_init(void) {

    static float x = 0;
    static float y = 0;

    k_list_node_loop(&world.list_node);

    world.component = NULL;

    world.world_x = &x;
    world.world_y = &y;

    world.parent = NULL;
    world.local_x  = 0;
    world.local_y  = 0;

    k_list_init(&world.list);
}

/* endregion */

/* region [position_set] */

static void update_position(struct k_position *self) {

    *(self->world_x) = *(self->parent->world_x) + self->local_x;
    *(self->world_y) = *(self->parent->world_y) + self->local_y;

    struct k_position *child;
    struct k_list *list = &self->list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        child = container_of(iter, struct k_position, list_node);

        update_position(child);
    }
}

void k_position_set_world_position(struct k_position *self, float world_x, float world_y) {

    self->local_x = world_x - *(self->parent->world_x);
    self->local_y = world_y - *(self->parent->world_y);

    update_position(self);
}

void k_position_set_local_position(struct k_position *self, float local_x, float local_y) {

    self->local_x = local_x;
    self->local_y = local_y;

    update_position(self);
}

/* endregion */

/* region [position_init] */

int position_init(struct k_component *component, void *params) {
    struct k_position *self = k_component_get_data(component);
    struct k_position_config *config = params;

    k_list_init(&self->list);

    struct k_position *parent = config->parent;
    if (NULL == parent)
        parent = &world;

    k_list_add_tail(&parent->list, &self->list_node);

    self->world_x = config->world_x;
    self->world_y = config->world_y;

    self->parent = parent;

    self->local_x = config->local_x;
    self->local_y = config->local_y;

    *(self->world_x) = *(self->parent->world_x) + self->local_x;
    *(self->world_y) = *(self->parent->world_y) + self->local_y;
    return 0;
}

void position_fini(struct k_component *component) {
    struct k_position *self = k_component_get_data(component);

    struct k_position *parent = self->parent;

    struct k_position *child;
    struct k_list *list = &self->list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        child = container_of(iter, struct k_position, list_node);

        k_list_add_tail(&parent->list, &child->list_node);
        child->parent = parent;
        child->local_x += self->local_x;
        child->local_y += self->local_y;
    }
}

/* endregion */

/* region [component_define] */

static struct k_component_type *K__COMPONENT_TYPE_POSITION = NULL;

int k__define_component_position(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct k_position);
    config.fn_init   = position_init;
    config.fn_fini   = position_fini;

    K__COMPONENT_TYPE_POSITION = k_component_define(NULL, &config);
    if (NULL == K__COMPONENT_TYPE_POSITION)
        return -1;

    world_init();
    return 0;
}

/* endregion */

/* region [object_add_component] */

struct k_position *k_object_add_position(struct k_object *object, const struct k_position_config *config) {

    if (NULL == config)
        return NULL;

    struct k_component *component = k_object_add_component(object, K__COMPONENT_TYPE_POSITION, (void*)config);
    if (NULL == component)
        return NULL;
    else
        return k_component_get_data(component);
}

void k_object_del_position(struct k_position *position) {
    if (NULL != position)
        k_object_del_component(position->component);
}

/* endregion */
