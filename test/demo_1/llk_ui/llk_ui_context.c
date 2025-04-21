#include <stdlib.h>

#include "k_log.h"

#include "k_game/core/k_canvas.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_elem_type_builtin.h"
#include "./llk_ui_elem_type.h"

/* region [create] */

static struct llk_ui_elem llk__ui_window = {
    .parent = NULL,
    .ui     = NULL,
    .x      = 0.0f,
    .y      = 0.0f,
};

static int llk__ui_registry_builtin_elem_types(struct llk_ui_context *ui) {

    if (0 != llk_ui_register_elem_type(ui, &llk__ui_elem_root_config))
        return -1;
    if (0 != llk_ui_register_elem_type(ui, &llk__ui_elem_box_config))
        return -1;
    if (0 != llk_ui_register_elem_type(ui, &llk__ui_elem_image_config))
        return -1;

    return 0;
}

struct llk_ui_context *llk_ui_create_context(void) {

    struct llk_ui_context *ui = NULL;
    struct k_mem_pool *mem_pool = NULL;
    struct k_str_map *map = NULL;

    ui = malloc(sizeof(struct llk_ui_context));
    if (NULL == ui)
        goto err;

    struct k_mem_pool_config config;
    config.fn_malloc        = malloc;
    config.fn_free          = free;
    config.alloc_size_align = 16;
    config.block_size_max   = sizeof(struct llk_ui_elem) * 2;
    config.alloc_chunk_size = 1024;
    mem_pool = k_mem_pool_construct(&ui->mem_pool, &config);
    if (NULL == mem_pool)
        goto err;

    map = k_str_map_construct(&ui->elem_type_map, NULL);
    if (NULL == map)
        goto err;

     if (0 != llk__ui_registry_builtin_elem_types(ui))
         goto err;

    float vw = k_canvas_ui_get_vw();
    float vh = k_canvas_ui_get_vh();

    struct llk_ui_elem *root = llk_ui_create_elem(ui, "root");
    if (NULL == root)
        goto err;

    root->w.unit = LLK_UI_UNIT_PX;
    root->w.specified_val = vw;
    root->w.computed_val  = vw;
    root->h.unit = LLK_UI_UNIT_PX;
    root->h.specified_val = vh;
    root->h.computed_val  = vh;
    root->parent = &llk__ui_window;

    ui->root = root;
    ui->vw = vw;
    ui->vh = vh;

    return ui;

err:
    if (NULL != map)
        k_str_map_destruct(map);

    if (NULL != mem_pool)
        k_mem_pool_destruct(mem_pool);

    if (NULL != ui)
        free(ui);

    k_log_error("Failed to create llk UI context");
    return NULL;
}

void llk_ui_destroy_context(struct llk_ui_context *ui) {
    k_str_map_destruct(&ui->elem_type_map);
    k_mem_pool_destruct(&ui->mem_pool);
    free(ui);
}

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui) {
    return ui->root;
}

/* endregion */

/* region [layout] */

void llk_ui_mark_layout_dirty(struct llk_ui_context *ui) {
    ui->layout_dirty = 1;
}

void llk__ui_layout(struct llk_ui_context *ui) {

    struct llk_ui_elem *child;
    struct k_list *child_list = &ui->root->child_list;
    struct k_list_node *iter;

    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);
        llk__ui_elem_measure(child);
    }

    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);
        llk__ui_elem_layout(child);
    }

    ui->layout_dirty = 0;
}

/* endregion */

/* region [draw] */

void llk_ui_draw(struct llk_ui_context *ui) {

    if (ui->layout_dirty) {
        llk__ui_layout(ui);
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &ui->root->child_list;
    struct k_list_node *iter;

    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);
        llk__ui_elem_measure(child);
    }

    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);
        llk__ui_elem_layout(child);
    }

    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);
        llk__ui_elem_draw(child);
    }
}

/* endregion */
