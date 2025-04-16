#ifndef YX_UI_ELEM_H
#define YX_UI_ELEM_H

#include "k_list.h"

#include "./yx_ui_elem_array.h"

struct yx_ui_context;
struct yx_ui_elem_v_tbl;

struct yx_ui_elem {

    struct yx_ui_context *ui;

    const char *elem_type;

    struct yx_ui_elem *parent;

    struct yx_ui_elem_array children;

    struct yx_ui_elem_v_tbl *v_tbl;
};

int yx__ui_elem_construct(struct yx_ui_elem *elem, struct yx_ui_context *ui);

void yx__ui_elem_destruct(struct yx_ui_elem *elem);

#endif
