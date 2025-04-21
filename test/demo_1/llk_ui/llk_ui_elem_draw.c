#include "./llk_ui_elem.h"
#include "./llk_ui_elem_type.h"

void llk__ui_elem_draw(struct llk_ui_elem *elem) {

    if (elem->type->fn_draw != NULL) {
        elem->type->fn_draw(elem);
    }

    struct llk_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct llk_ui_elem, sibling_link);

        llk__ui_elem_draw(child);
    }
}
