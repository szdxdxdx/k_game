#include "./yx_ui_internal.h"

struct yx_ui_elem *yx__ui_elem_construct(struct yx_ui_elem *elem, struct yx_ui_context *ui) {

    elem->ui = ui;

    elem->parent = NULL;
    yx_ui_elem_array_construct(&elem->children);

    elem->specified_w.unit  = YX_UNIT_PX;
    elem->specified_w.f_val = 0.0f;
    elem->specified_h.unit  = YX_UNIT_PX;
    elem->specified_h.f_val = 0.0f;

    elem->computed_w = -1.0f;
    elem->computed_h = -1.0f;

    elem->v_tbl = NULL;

    return elem;
}
