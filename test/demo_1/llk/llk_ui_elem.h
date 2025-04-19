#ifndef LLK_UI_ELEM_H
#define LLK_UI_ELEM_H

#include <stdint.h>

#include "k_list.h"

#include "./llk_ui_fwd.h"

enum llk_ui_unit {

    LLK_UI_UNIT_NO_VAL,

    LLK_UI_UNIT_PX,
    LLK_UI_UNIT_PERCENT,

    LLK_UI_UNIT_VW,
    LLK_UI_UNIT_VH,
};

struct llk_ui_float {

    float specified_val;
    enum llk_ui_unit unit;

    float computed_val;
};

#define llk_ui_float_init(val) \
( \
    (val).unit = LLK_UI_UNIT_NO_VAL, \
    (val).specified_val = 0.0f, \
    (val).computed_val  = 0.0f \
) \

struct llk_ui_elem {

    const char *name;

    struct llk_ui_context *ui;

    struct llk_ui_elem *parent;

    struct k_list child_list;

    struct k_list_node sibling_link;

    /* ------------------------------------------------------------------------ */

    struct llk_ui_float w;
    struct llk_ui_float h;

    struct llk_ui_float left;
    struct llk_ui_float right;
    struct llk_ui_float top;
    struct llk_ui_float bottom;

    float x;
    float y;

    /* ------------------------------------------------------------------------ */

    uint32_t background_color;
};

struct llk_ui_elem *llk__ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui);

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui);

int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child);

void llk__ui_elem_measure(struct llk_ui_elem *elem);

void llk__ui_elem_layout(struct llk_ui_elem *elem);

void llk__ui_elem_draw(struct llk_ui_elem *elem);

#endif
