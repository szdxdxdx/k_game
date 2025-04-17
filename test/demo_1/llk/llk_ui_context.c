#include <stdlib.h>

#include "./llk_ui_context.h"

struct llk_ui_context *llk_ui_create_context(void) {

    struct llk_ui_context *ui = malloc(sizeof(struct llk_ui_context));
    if (NULL == ui)
        return NULL;

    ui->root = NULL;

    return ui;
}

void llk_ui_draw(struct llk_ui_context *ui) {

}
