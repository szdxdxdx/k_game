#ifndef LLK_UI_ELEM_H
#define LLK_UI_ELEM_H

#include <stdint.h>

#include "./llk_ui_elem_array.h"

#include "./llk_ui_fwd.h"

enum llk_ui_unit {

    LLK_UI_UNIT_NO_VAL,

    LLK_UI_UNIT_PX,
    LLK_UI_UNIT_PERCENT,

    LLK_UI_UNIT_VW,
    LLK_UI_UNIT_VH,

    LLK_UI_UNIT_RGBA,
};

struct llk_ui_float {
    unsigned int is_computed : 1;
    enum llk_ui_unit unit : 7;
    float val;
    float computed_val;
};

struct llk_ui_u32 {
    unsigned int is_computed : 1;
    uint8_t unit;
    uint32_t val;
    uint32_t computed_val;
};

struct llk_ui_elem {

    struct llk_ui_context *ui;

    struct llk_ui_elem *parent;

    struct llk_ui_elem_array children;

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

struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui);

struct llk_ui_elem *llk_ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui);

int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child);

void llk_ui_elem_measure(struct llk_ui_elem *elem);

void llk_ui_elem_layout(struct llk_ui_elem *elem);

void llk_ui_elem_draw(struct llk_ui_elem *elem);

#endif
