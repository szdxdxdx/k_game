#include <stdlib.h>

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_root.h"

/* region [ui-context] */

struct llk_ui_context *llk_ui_create_context(void) {

    struct llk_ui_context *ui = malloc(sizeof(struct llk_ui_context));
    if (NULL == ui)
        return NULL;

    struct llk_ui_elem *root =llk_ui_create_root(ui);
    if (NULL == root)
        return NULL; /* TODO free */

    ui->root = root;
    ui->vw = 400.0f; /* tmp */
    ui->vh = 300.0f; /* tmp */

    return ui;
}

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui) {
    return ui->root;
}

void llk_ui_draw(struct llk_ui_context *ui) {

    llk_ui_elem_measure(ui->root);
    llk_ui_elem_layout(ui->root);
    llk_ui_elem_paint(ui->root);
}

/* endregion */
