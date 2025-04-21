#ifndef LLK_UI_FWD_H
#define LLK_UI_FWD_H

struct llk_ui_context;

struct llk_ui_elem;
struct llk_ui_elem_type;

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

#endif
