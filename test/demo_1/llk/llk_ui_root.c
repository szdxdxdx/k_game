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

    struct llk_ui_elem *root = llk_ui_create_elem(ui);
    if (NULL == root)
        return NULL;

    root->w.unit = LLK_UI_UNIT_PX;
    root->w.specified_val = ui->vw;
    root->h.computed_val  = ui->vw;

    root->h.unit = LLK_UI_UNIT_PX;
    root->h.specified_val = ui->vh;
    root->h.computed_val  = ui->vh;

    root->parent = &window;

    return root;
}
