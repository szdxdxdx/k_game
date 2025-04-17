#include "./llk_ui_elem.h"

struct llk_ui_elem *llk_ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui) {

    elem->elem_type_name = "";
    elem->ui = ui;

    elem->parent = NULL;
    llk_ui_elem_array_construct(&elem->children);

    elem->w.unit = LLK_UI_UNIT_PX;
    elem->w.val  = 0.0f;

    elem->h.unit = LLK_UI_UNIT_PX;
    elem->h.val  = 0.0f;

    elem->background_color.unit = LLK_UI_UNIT_RGBA;
    elem->background_color.val  = 0x00000000;

    elem->fn_measure = NULL;
    elem->fn_layout  = NULL;
    elem->fn_paint   = NULL;

    return elem;
}

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui) {

    struct llk_ui_elem *elem = malloc(sizeof(struct llk_ui_elem));
    if (NULL == elem)
        return NULL;

    return llk_ui_construct_elem(elem, ui);
}

int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child) {

    if (NULL == parent || NULL == child)
        return -1;
    if (NULL != child->parent)
        return -1;
    if (child->ui != parent->ui)
        return -1;

    if (0 != llk_ui_elem_array_push_back(&parent->children, child))
        return -1;

    child->parent = parent;
    return 0;
}

void llk_ui_elem_measure(struct llk_ui_elem *elem) {

    struct llk_ui_elem *parent = elem->parent;

    size_t i = 0;
    for (; i < elem->children.size; i++) {
        struct llk_ui_elem *child = elem->children.storage[i];
        llk_ui_elem_measure(child);
    }

    if (NULL != elem->fn_measure) {
        elem->fn_measure(elem);
    }
}
