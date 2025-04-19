#include <stdlib.h>

#include "./llk_ui_root.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_context.h"

struct llk_ui_root {
    struct llk_ui_elem super;
};

static struct llk_ui_elem window = {
    .x = 0.0f,
    .y = 0.0f
};

struct llk_ui_elem *llk_ui_create_root(struct llk_ui_context *ui) {

    struct llk_ui_root *root = malloc(sizeof(struct llk_ui_root));
    if (NULL == root)
        return NULL;

    struct llk_ui_elem *super = &root->super;
    llk_ui_construct_elem(super, ui);

    super->w.unit = LLK_UI_UNIT_VW;
    super->w.specified_val = 1.0f;

    super->h.unit = LLK_UI_UNIT_VH;
    super->h.specified_val = 1.0f;

    super->parent = &window;

    super->x = 0.0f;
    super->y = 0.0f;

    ui->root = &root->super;

    return &root->super;
}
