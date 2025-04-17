#ifndef LLK_UI_ELEM_H
#define LLK_UI_ELEM_H

#include <stdint.h>

#include "./llk_ui_elem_array.h"

#include "./llk_ui_fwd.h"

enum llk_ui_unit {

    LLK_UI_UNIT_NONE,

    LLK_UI_UNIT_INHERIT,

    LLK_UI_UNIT_PX,
    LLK_UI_UNIT_PERCENT,
    LLK_UI_UNIT_AUTO,
    LLK_UI_UNIT_FIT_CONTENT,
    LLK_UI_UNIT_VW,
    LLK_UI_UNIT_VH,

    LLK_UI_UNIT_RGBA,
};

struct llk_ui_float {
    enum llk_ui_unit unit;
    float val;
    float computed_val;
};

struct llk_ui_u32 {
    enum llk_ui_unit unit;
    uint32_t val;
    uint32_t computed_val;
};

struct llk_ui_elem {

    const char *elem_type_name;

    struct llk_ui_context *ui;

    struct llk_ui_elem *parent;

    struct llk_ui_elem_array children;

    /* ------------------------------------------------------------------------ */

    struct llk_ui_float margin_top;
    struct llk_ui_float margin_right;
    struct llk_ui_float margin_bottom;
    struct llk_ui_float margin_left;

    struct llk_ui_float border_top;
    struct llk_ui_float border_right;
    struct llk_ui_float border_bottom;
    struct llk_ui_float border_left;

    struct llk_ui_float padding_top;
    struct llk_ui_float padding_right;
    struct llk_ui_float padding_bottom;
    struct llk_ui_float padding_left;

    struct llk_ui_float w;
    struct llk_ui_float min_w;
    struct llk_ui_float max_w;

    struct llk_ui_float h;
    struct llk_ui_float min_h;
    struct llk_ui_float max_h;
    float x;
    float y;

    /* ------------------------------------------------------------------------ */

    void (*fn_measure)(struct llk_ui_elem *elem);

    void (*fn_layout)(struct llk_ui_elem *elem);

    /* ------------------------------------------------------------------------ */

    struct llk_ui_u32 background_color;

    void (*fn_paint)(struct llk_ui_elem *elem);
};

struct llk_ui_elem *llk_ui_construct_elem(struct llk_ui_elem *elem, struct llk_ui_context *ui);

int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child);

#endif
