#include "./_internal.h"

/* region [behavior_tree_create] */

struct k_behavior_tree *k_behavior_tree_create(void) {

    struct k_object *object = k_object_create(sizeof(struct k_behavior_tree));
    if (NULL == object)
        return NULL;

    struct k_behavior_tree *tree = k_object_get_data(object);
    tree->object = object;

    /* ... */

    return tree;
}

void k_behavior_tree_destroy(struct k_behavior_tree *tree) {

    if (NULL != tree)
        k_object_destroy(tree->object);
}

/* endregion */
