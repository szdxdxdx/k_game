#include <stdlib.h>

#include "k_array.h"

#include "./_internal.h"

struct k_behavior_tree_builder {

    struct k_behavior_tree *tree;

    struct k_behavior_tree **result;

    struct k_array stack;

    /* 标记构建过程中是否出现错误。若某步出现错误，往后的步骤都快速返回 */
    int failed;

    /* 标记构建是否已开始。用于控制进入或退出 `k_bt_builder()` 宏的 for 循环  */
    int started;
};

/* region [stack] */

struct k_bt_builder_stack_node {

    struct k_behavior_tree_node *node;

    int pop_count;
};

static int is_empty(struct k_behavior_tree_builder *builder) {
    return builder->stack.size == 0;
}

static void push(struct k_behavior_tree_builder *builder, struct k_behavior_tree_node *node) {

    if (builder->failed)
        return;

    struct k_array *arr = &builder->stack;
    struct k_bt_builder_stack_node *stack_node = k_array_shift_right(arr, arr->size, 1);
    if (NULL == stack_node) {
        builder->failed = 1;
        return;
    }

    stack_node->node = node;
    stack_node->pop_count = 1;
}

static struct k_behavior_tree_node *top(struct k_behavior_tree_builder *builder) {
    struct k_bt_builder_stack_node *node = k_array_get_elem_addr(&builder->stack, builder->stack.size - 1);
    return node->node;
}

static int pop(struct k_behavior_tree_builder *builder) {

    if (builder->failed)
        return 0;

    struct k_array *arr = &builder->stack;
    struct k_bt_builder_stack_node *top = k_array_get_elem_addr(arr, arr->size - 1);
    if (0 == top->pop_count) {
        k_array_pop_back(&builder->stack);
        return 0;
    } else {
        top->pop_count -= 1;
        return 1;
    }
}

/* endregion */

/* region [builder_create] */

static struct k_behavior_tree_builder *k__behavior_tree_builder_create(struct k_behavior_tree **get_tree) {

    struct k_behavior_tree *tree = k_behavior_tree_create();
    if (NULL == tree)
        goto err;

    struct k_behavior_tree_builder *builder = malloc(sizeof(struct k_behavior_tree_builder));
    if (NULL == builder) {
        k_behavior_tree_destroy(tree);
        goto err;
    }

    builder->tree    = tree;
    builder->result  = get_tree;
    builder->failed  = 0;
    builder->started = 0;

    struct k_array_config config;
    config.fn_malloc     = malloc;
    config.fn_free       = free;
    config.elem_size     = sizeof(struct k_bt_builder_stack_node);
    config.init_capacity = 16;
    if (NULL == k_array_construct(&builder->stack, &config)) {
        k_behavior_tree_destroy(tree);
        free(builder);
        goto err;
    }

    struct k_behavior_tree_node *root = k_behavior_tree_get_root(tree);

    push(builder, root);
    return builder;

err:
    *get_tree = NULL;
    return NULL;
}

static void k__behavior_tree_builder_destroy(struct k_behavior_tree_builder *builder) {

    if (builder->failed) {
        k_behavior_tree_destroy(builder->tree);
        *(builder->result) = NULL;
    }
    else {
        if (NULL == builder->result) {
            *(builder->result) = builder->tree;
        }
    }

    k_array_destruct(&builder->stack);
    free(builder);
}

/* endregion */

/* region [macro] */

/* region [builder] */

struct k_behavior_tree_builder *k__behavior_tree_builder_begin(struct k_behavior_tree **get_tree) {
    return k__behavior_tree_builder_create(get_tree);
}

int k__behavior_tree_builder_end(struct k_behavior_tree_builder *builder) {

    if ( ! builder->started) {
        return 1;
    }
    else {
        k__behavior_tree_builder_destroy(builder);
        return 0;
    }
}

int k__behavior_tree_builder_pop(struct k_behavior_tree_builder *builder) {
    return pop(builder);
}

/* endregion */

/* region [execution] */

void k__behavior_tree_builder_action(struct k_behavior_tree_builder *builder, void *data, enum k_behavior_tree_status (*fn_tick)(void *data)) {

    if (builder->failed)
        return;

    if (NULL == k_behavior_tree_add_action(top(builder), data, fn_tick))
        builder->failed = 1;
}

void k__behavior_tree_builder_condition(struct k_behavior_tree_builder *builder, void *data, enum k_behavior_tree_status (*fn_tick)(void *data)) {

    if (builder->failed)
        return;

    if (NULL == k_behavior_tree_add_condition(top(builder), data, fn_tick))
        builder->failed = 1;
}

/* endregion */

/* region [control] */

void k__behavior_tree_builder_sequence(struct k_behavior_tree_builder *builder) {

    if (builder->failed)
        return;

    struct k_behavior_tree_node *new_node = k_behavior_tree_add_sequence(top(builder));
    if (NULL == new_node) {
        builder->failed = 1;
        return;
    }

    push(builder, new_node);
}

void k__behavior_tree_builder_selector(struct k_behavior_tree_builder *builder) {

    if (builder->failed)
        return;

    struct k_behavior_tree_node *new_node = k_behavior_tree_add_selector(top(builder));
    if (NULL == new_node) {
        builder->failed = 1;
        return;
    }

    push(builder, new_node);
}

void k__behavior_tree_builder_parallel(struct k_behavior_tree_builder *builder) {

    if (builder->failed)
        return;

    struct k_behavior_tree_node *new_node = k_behavior_tree_add_parallel(top(builder));
    if (NULL == new_node) {
        builder->failed = 1;
        return;
    }

    push(builder, new_node);
}

/* endregion */

/* region [decorator] */

void k__behavior_tree_builder_inverter(struct k_behavior_tree_builder *builder) {

    if (builder->failed)
        return;

    struct k_behavior_tree_node *new_node = k_behavior_tree_add_inverter(top(builder));
    if (NULL == new_node) {
        builder->failed = 1;
        return;
    }

    push(builder, new_node);
}

void k__behavior_tree_builder_force_success(struct k_behavior_tree_builder *builder) {

    if (builder->failed)
        return;

    struct k_behavior_tree_node *new_node = k_behavior_tree_add_force_success(top(builder));
    if (NULL == new_node) {
        builder->failed = 1;
        return;
    }

    push(builder, new_node);
}

void k__behavior_tree_builder_force_failure(struct k_behavior_tree_builder *builder) {

    if (builder->failed)
        return;

    struct k_behavior_tree_node *new_node = k_behavior_tree_add_force_failure(top(builder));
    if (NULL == new_node) {
        builder->failed = 1;
        return;
    }

    push(builder, new_node);
}

/* endregion */

/* endregion */
