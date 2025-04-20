#include <string.h>

#include "./llk_ui_fwd.h"
#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_val_parser.h"

/* region [size] */

static int llk__ui_elem_set_attr_w(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    llk_ui_float_set(&elem->w, float_val, unit);
    return 0;
}

static int llk__ui_elem_set_attr_h(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    llk_ui_float_set(&elem->h, float_val, unit);
    return 0;
}

/* endregion */

/* region [position] */

static int llk__ui_elem_set_attr_left(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    llk_ui_float_set(&elem->left, float_val, unit);
    return 0;
}

static int llk__ui_elem_set_attr_right(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    llk_ui_float_set(&elem->right, float_val, unit);
    return 0;
}

static int llk__ui_elem_set_attr_top(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    llk_ui_float_set(&elem->top, float_val, unit);
    return 0;
}

static int llk__ui_elem_set_attr_bottom(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    llk_ui_float_set(&elem->bottom, float_val, unit);
    return 0;
}

/* endregion */

/* region [background] */

static int llk__ui_elem_set_attr_background_color(struct llk_ui_elem *elem, const char *val) {

    uint32_t u32_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_color_val(val, &u32_val))
        return -1;

    elem->background_color = u32_val;
    return 0;
}

/* endregion */

int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {

#define llk__ui_key_match(key, str) \
    (0 == strncmp((key), "" str "", sizeof(str) - 1))

    if (llk__ui_key_match(key, "w"))
        return llk__ui_elem_set_attr_w(elem, val);
    if (llk__ui_key_match(key, "h"))
        return llk__ui_elem_set_attr_h(elem, val);

    if (llk__ui_key_match(key, "left"))
        return llk__ui_elem_set_attr_left(elem, val);
    if (llk__ui_key_match(key, "right"))
        return llk__ui_elem_set_attr_right(elem, val);
    if (llk__ui_key_match(key, "top"))
        return llk__ui_elem_set_attr_top(elem, val);
    if (llk__ui_key_match(key, "bottom"))
        return llk__ui_elem_set_attr_bottom(elem, val);

    if (llk__ui_key_match(key, "background-color"))
        return llk__ui_elem_set_attr_background_color(elem, val);

#undef llk__ui_key_match

    return -1;
}
