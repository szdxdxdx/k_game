#ifndef YX_BEHAVIOR_TREE_PUBLIC_H
#define YX_BEHAVIOR_TREE_PUBLIC_H

struct yx_behavior_tree;

struct yx_behavior_tree *yx_behavior_tree_create(void);

void yx_behavior_tree_destroy(struct yx_behavior_tree *tree);

/* ------------------------------------------------------------------------ */

void yx_behavior_tree_demo(void);

#endif
