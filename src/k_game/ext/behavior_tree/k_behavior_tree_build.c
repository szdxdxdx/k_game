#include "./_internal.h"

struct k_bt_node *k_bt_get_root(struct k_behavior_tree *tree) {
    return tree->root;
}
