#include <stdlib.h>

#include "./yx_ui_internal.h"

struct yx_ui_context *yx_ui_create_context(void) {

    struct yx_ui_context *ui = malloc(sizeof(struct yx_ui_context));
    if (NULL == ui)
        return NULL;

    ui->root = NULL;
    return ui;
}
