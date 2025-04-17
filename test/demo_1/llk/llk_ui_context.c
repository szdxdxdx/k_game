#include <stdlib.h>

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"

/* region [root] */

struct llk_ui_root {
    struct llk_ui_elem super;
};

struct llk_ui_elem *llk_ui_create_root(struct llk_ui_context *ui) {

    struct llk_ui_root *root = malloc(sizeof(struct llk_ui_root));
    if (NULL == root)
        return NULL;

    llk_ui_construct_elem(&root->super, ui);
    root->super.w.unit = LLK_UI_UNIT_VW;
    root->super.w.val  = 100.0f;
    root->super.h.unit = LLK_UI_UNIT_VH;
    root->super.h.val  = 100.0f;

    ui->root = &root->super;

    return ui->root;
}

/* endregion */

/* region [ui-context] */

struct llk_ui_context *llk_ui_create_context(void) {

    struct llk_ui_context *ui = malloc(sizeof(struct llk_ui_context));
    if (NULL == ui)
        return NULL;

    struct llk_ui_elem *root =llk_ui_create_root(ui);
    if (NULL == root)
        return NULL; /* TODO free */

    ui->root = root;

    return ui;
}

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui) {
    return ui->root;
}

void llk_ui_draw(struct llk_ui_context *ui) {

}

/* endregion */
