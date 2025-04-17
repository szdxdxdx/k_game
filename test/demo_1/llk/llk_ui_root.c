#include "./llk_ui_root.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_context.h"

static void root_measure(struct llk_ui_elem *elem);
static void root_layout(struct llk_ui_elem *elem);
static void root_paint(struct llk_ui_elem *elem);

struct llk_ui_root {
    struct llk_ui_elem super;
};

struct llk_ui_elem *llk_ui_create_root(struct llk_ui_context *ui) {

    struct llk_ui_root *root = malloc(sizeof(struct llk_ui_root));
    if (NULL == root)
        return NULL;

    struct llk_ui_elem *super = &root->super;
    llk_ui_construct_elem(super, ui);
    super->w.unit = LLK_UI_UNIT_VW;
    super->w.val  = 1.0f;
    super->h.unit = LLK_UI_UNIT_VH;
    super->h.val  = 1.0f;

    super->fn_measure = root_measure;
    super->fn_layout  = root_layout;
    super->fn_paint   = root_paint;

    ui->root = &root->super;

    return &root->super;
}

void root_measure(struct llk_ui_elem *elem) {

}

void root_layout(struct llk_ui_elem *elem) {

}

void root_paint(struct llk_ui_elem *elem) {

}
