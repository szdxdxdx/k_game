#include <stdlib.h>

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
    if (NULL == ui)
        return NULL;

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

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui) {
    return ui->root;
}

void llk_ui_draw(struct llk_ui_context *ui) {

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
