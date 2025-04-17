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

    return ui;
}

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui) {
    return ui->root;
}

void llk_ui_draw(struct llk_ui_context *ui) {

}

/* endregion */
