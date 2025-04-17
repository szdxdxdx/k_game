#include "./llk_ui_elem.h"

struct llk_ui_elem *llk_ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui) {

    elem->elem_type_name = "";
    elem->ui = ui;

    elem->parent = NULL;
    llk_ui_elem_array_construct(&elem->children);

    elem->margin_top.unit    = LLK_UI_UNIT_NONE;
    elem->margin_right.unit  = LLK_UI_UNIT_NONE;
    elem->margin_bottom.unit = LLK_UI_UNIT_NONE;
    elem->margin_left.unit   = LLK_UI_UNIT_NONE;

    elem->border_top.unit    = LLK_UI_UNIT_NONE;
    elem->border_right.unit  = LLK_UI_UNIT_NONE;
    elem->border_bottom.unit = LLK_UI_UNIT_NONE;
    elem->border_left.unit   = LLK_UI_UNIT_NONE;

    elem->padding_top.unit    = LLK_UI_UNIT_NONE;
    elem->padding_right.unit  = LLK_UI_UNIT_NONE;
    elem->padding_bottom.unit = LLK_UI_UNIT_NONE;
    elem->padding_left.unit   = LLK_UI_UNIT_NONE;

    elem->w.unit = LLK_UI_UNIT_PERCENT;
    elem->w.val  = 100.0f;

    elem->min_w.unit = LLK_UI_UNIT_NONE;
    elem->max_w.unit = LLK_UI_UNIT_NONE;

    elem->h.unit = LLK_UI_UNIT_FIT_CONTENT;

    elem->min_h.unit = LLK_UI_UNIT_NONE;
    elem->max_h.unit = LLK_UI_UNIT_NONE;

    elem->fn_measure = NULL;

    elem->fn_layout = NULL;

    elem->background_color.unit = LLK_UI_UNIT_RGBA;
    elem->background_color.val  = 0x00000000;

    elem->fn_paint = NULL;

    return elem;
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
