#ifndef LLK_UI_CONTEXT_H
#define LLK_UI_CONTEXT_H

#include "./llk_ui_fwd.h"

struct llk_ui_context {

    struct llk_ui_elem *root;
};

struct llk_ui_context *llk_ui_create_context(void);

void llk_ui_draw(struct llk_ui_context *ui);

#endif
