#include <stdlib.h>

#include "./yx_ui_internal.h"

struct yx_ui_context *yx_ui_create_context(void) {

    struct yx_ui_context *ui = malloc(sizeof(struct yx_ui_context));
    if (NULL == ui)
        return NULL;

    ui->root = NULL;
    return ui;
}

int yx_ui_set_root(struct yx_ui_context *ui, struct yx_ui_elem *root) {
    ui->root = root;
    return 0;
}

void yx_ui_draw(struct yx_ui_context *ui) {

    struct yx_ui_elem *root = ui->root;
    root->v_tbl->fn_measure(root);
    root->v_tbl->fn_layout(root);
    root->v_tbl->fn_draw(root);
}
