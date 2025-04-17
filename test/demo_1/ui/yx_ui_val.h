#ifndef YX_UI_VAL_H
#define YX_UI_VAL_H

#include <stdint.h>

enum yx_ui_unit {
    YX_UNIT_PX,
    YX_UNIT_PERCENT,
    YX_UNIT_AUTO,
    YX_UNIT_FIT_CONTENT,
};


struct yx_ui_val {
    uint8_t unit;
    float val;
};

#endif
