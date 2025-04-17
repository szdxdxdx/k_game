
#include "./yx_ui_internal.h"

struct yx_ui_elem_box {
    struct yx_ui_elem super;
};

/* region [v_tbl] */

static void box_measure(struct yx_ui_elem *elem) {

}

static void box_layout(struct yx_ui_elem *elem) {

}

static void box_paint(struct yx_ui_elem *elem) {

}

static struct yx_ui_elem_v_tbl box_v_tbl = {
    .fn_measure = box_measure,
    .fn_layout  = box_layout,
    .fn_paint   = box_paint,
};

/* endregion */

/* region [create] */

struct yx_ui_elem *yx_ui_create_elem_box(struct yx_ui_context *ui) {

    struct yx_ui_elem_box *box = malloc(sizeof(struct yx_ui_elem_box));
    if (NULL == box)
        return NULL;

    yx__ui_elem_construct(&box->super, ui);
    box->super.v_tbl = &box_v_tbl;

    return &box->super;
}

/* endregion */
