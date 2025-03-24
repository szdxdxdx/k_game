#include "_internal.h"

/* region [world] */

static struct k_position world;

static void world_init(void) {

    static float x = 0;
    static float y = 0;

    k_list_node_loop(&world.list_node);

    world.component = NULL;

    world.x = &x;
    world.y = &y;

    world.parent = NULL;
    world.rel_x  = 0;
    world.rel_y  = 0;

    k_list_init(&world.list);
}

/* endregion */

/* region [position_set] */

static void update_position(struct k_position *self) {

    *(self->x) = *(self->parent->x) + self->rel_x;
    *(self->y) = *(self->parent->y) + self->rel_y;

    struct k_position *child;
    struct k_list *list = &self->list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        child = container_of(iter, struct k_position, list_node);

        update_position(child);
    }
}

void k_position_set(struct k_position *self, float x, float y) {

    self->rel_x = x - *(self->parent->x);
    self->rel_y = y - *(self->parent->y);

    update_position(self);
}

void k_position_set_rel(struct k_position *self, float rel_x, float rel_y) {

    self->rel_x = rel_x;
    self->rel_y = rel_y;

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

    self->x = config->x;
    self->y = config->y;

    self->parent = parent;
    self->rel_x = config->rel_x;
    self->rel_y = config->rel_y;

    *(self->x) = *(self->parent->x) + self->rel_x;
    *(self->y) = *(self->parent->y) + self->rel_y;
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
        child->rel_x += self->rel_x;
        child->rel_y += self->rel_y;
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

    K__COMPONENT_TYPE_POSITION = k_define_component(NULL, &config);
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
