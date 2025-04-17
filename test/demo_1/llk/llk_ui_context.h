#ifndef LLK_UI_CONTEXT_H
#define LLK_UI_CONTEXT_H

#include "./llk_ui_fwd.h"

struct llk_ui_context {

    struct llk_ui_elem *root;

    float vw;
    float vh;
};

struct llk_ui_context *llk_ui_create_context(void);

struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui);

void llk_ui_set_root(struct llk_ui_context *ui, struct llk_ui_elem *root);

void llk_ui_draw(struct llk_ui_context *ui);

#endif
