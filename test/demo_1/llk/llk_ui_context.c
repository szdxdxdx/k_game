#include <stdlib.h>

#include "k_log.h"

#include "k_game/core/k_canvas.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"

static struct llk_ui_elem llk__ui_window = {
    .parent = NULL,
    .ui = NULL,
    .x = 0,
    .y = 0,
};

struct llk_ui_context *llk_ui_create_context(void) {

    struct llk_ui_context *ui = malloc(sizeof(struct llk_ui_context));
    if (NULL == ui) {
        k_log_error("Failed to create llk UI context, malloc() failed");
        return NULL;
    }

    struct k_mem_pool_config config;
    config.fn_malloc        = malloc;
    config.fn_free          = free;
    config.alloc_size_align = 16;
    config.block_size_max   = sizeof(struct llk_ui_elem) * 2;
    config.alloc_chunk_size = 1024;
    if (NULL == k_mem_pool_construct(&ui->mem_pool, &config)) {
        free(ui);
        k_log_error("Failed to create llk UI context");
        return NULL;
    }

    float vw = k_canvas_ui_get_vw();
    float vh = k_canvas_ui_get_vh();

    struct llk_ui_elem *root = llk_ui_create_elem(ui);
    {
        root->w.unit = LLK_UI_UNIT_PX;
        root->w.specified_val = vw;
        root->w.computed_val  = vw;

        root->h.unit = LLK_UI_UNIT_PX;
        root->h.specified_val = vh;
        root->h.computed_val  = vh;

        root->parent = &llk__ui_window;
    }

    ui->root = root;
    ui->vw = vw;
    ui->vh = vh;

    return ui;
}

void llk_ui_destroy_context(struct llk_ui_context *ui) {
    k_mem_pool_destruct(&ui->mem_pool);
    free(ui);
}

void *llk__ui_mem_alloc(struct llk_ui_context *ui, size_t size) {
    return k_mem_pool_alloc(&ui->mem_pool, size);
}

void llk__ui_mem_free(void *p) {
    return k_mem_pool_free(p);
}

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui) {
    return ui->root;
}

void llk_ui_draw(struct llk_ui_context *ui) {

    if (NULL == ui)
        return;

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
