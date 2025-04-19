#include <stdlib.h>

#include "k_game/core/k_window.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"

/* region [ui_context] */

struct llk_ui_context *llk_ui_create_context(void) {

    struct llk_ui_context *ui = malloc(sizeof(struct llk_ui_context));
    if (NULL == ui)
        return NULL;

    struct llk_ui_elem *root =llk_ui_create_elem(ui);
    if (NULL == root) {
        free(ui);
        return NULL;
    }

    ui->root = root;
    ui->vw = k_window_get_width();
    ui->vh = k_window_get_height();

    return ui;
}

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui) {
    return ui->root;
}

void llk_ui_draw(struct llk_ui_context *ui) {

    llk_ui_elem_measure(ui->root);
    llk_ui_elem_layout(ui->root);
    llk_ui_elem_draw(ui->root);
}

/* endregion */
