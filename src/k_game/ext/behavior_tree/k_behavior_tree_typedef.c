#include "./_internal.h"

/* region [tree_init] */

int k__behavior_tree_init(struct k_component *component, void *params) {
    (void)params;
    struct k_behavior_tree *tree = k_component_get_data(component);

    tree->component = component;

    return 0;
}

/* endregion */

/* region [component_define] */

static struct k_component_type *k__component_type_behavior_tree = NULL;

int k__define_component_sprite_renderer(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct k_behavior_tree);
    config.fn_init = k__behavior_tree_init;

    struct k_component_type *type = k_component_define(NULL, &config);
    if (NULL == type)
        return -1;

    k__component_type_behavior_tree = type;
    return 0;
}

/* endregion */

/* region [object_add_component] */

struct k_behavior_tree *k_object_add_behavior_tree(struct k_object *object) {

    struct k_component *component = k_object_add_component(object, k__component_type_behavior_tree, NULL);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void k_object_del_behavior_tree(struct k_behavior_tree *tree) {
    if (NULL != tree)
        k_object_del_component(tree->component);
}

/* endregion */
