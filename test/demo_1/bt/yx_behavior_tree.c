#include "./_internal.h"

static void yx_behavior_tree_tick(struct k_object *b) {
    struct yx_behavior_tree *tree = k_object_get_data(b);

    printf("yx_behavior_tree_tick\n");
}

struct yx_behavior_tree *yx_behavior_tree_create(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_behavior_tree));
    if (NULL == object)
        return NULL;

    if (NULL == k_object_add_step_callback(object, yx_behavior_tree_tick)) {
        k_object_destroy(object);
        return NULL;
    }

    struct yx_behavior_tree *tree = k_object_get_data(object);
    tree->object = object;

    return tree;
}

void yx_behavior_tree_destroy(struct yx_behavior_tree *tree) {
    if (NULL != tree)
        k_object_destroy(tree->object);
}

void yx_behavior_tree_demo(void) {
    struct yx_behavior_tree *tree = yx_behavior_tree_create();
}
