#include <string.h>

#include "k_log.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem.h"
#include "./llk_ui_elem_type.h"
#include "./llk_ui_elem_set_attr.h"

/* region [size] */

static int llk__ui_elem_set_attr_w(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    if (float_val < 0.0f)
        return -1;

    elem->w.specified_val = float_val;
    elem->w.unit = unit;

    return 0;
}

static int llk__ui_elem_set_attr_h(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    if (float_val < 0.0f)
        return -1;

    elem->h.specified_val = float_val;
    elem->h.unit = unit;

    return 0;
}

/* endregion */

/* region [position] */

static int llk__ui_elem_set_attr_left(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    elem->left.specified_val = float_val;
    elem->left.unit = unit;

    return 0;
}

static int llk__ui_elem_set_attr_right(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    elem->right.specified_val = float_val;
    elem->right.unit = unit;

    return 0;
}

static int llk__ui_elem_set_attr_top(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    elem->top.specified_val = float_val;
    elem->top.unit = unit;

    return 0;
}

static int llk__ui_elem_set_attr_bottom(struct llk_ui_elem *elem, const char *val) {

    float float_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_length_val(val, &float_val, &unit))
        return -1;

    elem->bottom.specified_val = float_val;
    elem->bottom.unit = unit;

    return 0;
}

/* endregion */

/* region [color] */

static int llk__ui_elem_set_attr_background_color(struct llk_ui_elem *elem, const char *val) {

    uint32_t u32_val;
    if (0 != llk__ui_parse_color_val(val, &u32_val))
        return -1;

    elem->background_color = u32_val;

    return 0;
}

int llk__ui_elem_set_attr_default(struct llk_ui_elem *elem, const char *key, const char *val) {

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

    return -1;
}

/* endregion */

int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {

    int result;
    if (NULL == elem->type->fn_set_attr) {
        result = llk__ui_elem_set_attr_default(elem, key, val);
    } else {
        result = elem->type->fn_set_attr(elem, key, val);
    }

    if (0 != result) {
        k_log_warn("llk UI element `%s`: Unsupported attribute or invalid value, key: `%s` value: `%s`", elem->type->type_name, key, val);
        return -1;
    }

    return 0;
}
