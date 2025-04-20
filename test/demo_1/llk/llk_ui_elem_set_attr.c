#include <string.h>

#include "./llk_ui_fwd.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_val_parser.h"

int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {

    if (0 == strncmp(key, "w", 1)) {
        float float_val;
        enum llk_ui_unit unit;
        if (0 == llk__ui_parse_length_val(val, &float_val, &unit)) {
            llk_ui_float_set(&elem->w, float_val, unit);
            return 0;
        }
    }
    else if (0 == strncmp(key, "h", 1)) {
        float float_val;
        enum llk_ui_unit unit;
        if (0 == llk__ui_parse_length_val(val, &float_val, &unit)) {
            llk_ui_float_set(&elem->h, float_val, unit);
            return 0;
        }
    }
    else if (0 == strncmp(key, "left", 4)) {
        float float_val;
        enum llk_ui_unit unit;
        if (0 == llk__ui_parse_length_val(val, &float_val, &unit)) {
            llk_ui_float_set(&elem->left, float_val, unit);
            return 0;
        }
    }
    else if (0 == strncmp(key, "right", 5)) {
        float float_val;
        enum llk_ui_unit unit;
        if (0 == llk__ui_parse_length_val(val, &float_val, &unit)) {
            llk_ui_float_set(&elem->right, float_val, unit);
            return 0;
        }
    }
    else if (0 == strncmp(key, "top", 3)) {
        float float_val;
        enum llk_ui_unit unit;
        if (0 == llk__ui_parse_length_val(val, &float_val, &unit)) {
            llk_ui_float_set(&elem->top, float_val, unit);
            return 0;
        }
    }
    else if (0 == strncmp(key, "bottom", 6)) {
        float float_val;
        enum llk_ui_unit unit;
        if (0 == llk__ui_parse_length_val(val, &float_val, &unit)) {
            llk_ui_float_set(&elem->bottom, float_val, unit);
            return 0;
        }
    }
    else if (0 == strncmp(key, "background-color", 16)) {
        uint32_t u32_val;
        if (0 == llk__ui_parse_color_val(val, &u32_val)) {
            elem->background_color = u32_val;
            return 0;
        }
    }

    return -1;
}
