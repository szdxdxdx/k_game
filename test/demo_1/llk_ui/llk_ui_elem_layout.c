#include "./llk_ui_elem.h"

void llk__ui_elem_layout(struct llk_ui_elem *elem) {

    struct llk_ui_elem *parent = elem->parent;

    if (elem->left.unit != LLK_UI_UNIT_NO_VAL) {
        elem->x = parent->x + elem->left.computed_val;
    }
    else if (elem->right.unit != LLK_UI_UNIT_NO_VAL) {
        elem->x = parent->x + parent->w.computed_val - elem->w.computed_val - elem->right.computed_val;
    }
    else {
        elem->x = parent->x;
    }

    if (elem->top.unit != LLK_UI_UNIT_NO_VAL) {
        elem->y = parent->y + elem->top.computed_val;
    }
    else if (elem->bottom.unit != LLK_UI_UNIT_NO_VAL) {
        elem->y = parent->y + parent->h.computed_val - elem->h.computed_val - elem->bottom.computed_val;
    }
    else {
        elem->y = parent->y;
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_layout(child);
    }
}
