#ifndef YX_UI_ELEM_BUTTON_H
#define YX_UI_ELEM_BUTTON_H

#include <stdint.h>

#include "./yx_ui_elem.h"

struct yx_ui_button {
    struct yx_ui_elem super;

    float x;
    float y;
    float w;
    float h;
    uint32_t background_color;
};



#endif
