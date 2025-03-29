#include "./_internal.h"

/* region [behavior_tree_create] */

static int check_config(const struct k_behavior_tree_config *config) {

    if (NULL == config)
        return -1;
    if (SIZE_MAX - sizeof(struct k_behavior_tree) <= config->data_size)
        return -1;

    return 0;
}

struct k_behavior_tree *k_behavior_tree_create(const struct k_behavior_tree_config *config) {

    if (0 != check_config(config))
        return NULL;

    struct k_object *object = k_object_create(sizeof(struct k_behavior_tree) + config->data_size);
    if (NULL == object)
        return NULL;

    struct k_behavior_tree *tree = k_object_get_data(object);
    tree->object = object;


    return tree;
}

void k_behavior_tree_destroy(struct k_behavior_tree *tree) {

    if (NULL != tree)
        k_object_destroy(tree->object);
}

/* endregion */
