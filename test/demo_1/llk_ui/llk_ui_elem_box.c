#include "k_log.h"

#include "k_game/core/k_canvas.h"

#include "./llk_ui_elem_type_builtin.h"
#include "./llk_ui_elem_type.h"
#include "./llk_ui_elem_set_attr.h"

static int box_init(struct llk_ui_elem *elem) {

    return 0;
}

static int box_set_attr_border_color(struct llk_ui_elem *elem, const char *val) {

    uint32_t u32_val;
    if (0 != llk__ui_parse_color_val(val, &u32_val))
        goto err;

    elem->border_color = u32_val;

    return 0;

err:
    k_log_warn("invalid val: `%s`", val);
    return -1;
}

static int box_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {

    if (llk__ui_key_match(key, "border-color"))
        return box_set_attr_border_color(elem, val);

    return llk__ui_elem_set_attr_default(elem, key, val);
}

static void box_draw(struct llk_ui_elem *elem) {

    k_canvas_set_draw_color_rgba(elem->background_color);
    k_canvas_ui_fill_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);

    k_canvas_set_draw_color_rgba(elem->border_color);
    k_canvas_ui_draw_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
}

struct llk_ui_elem_type_config llk__ui_elem_box_config = {
    .type_name   = "box",
    .data_size   = 0,
    .fn_init     = box_init,
    .fn_fini     = NULL,
    .fn_set_attr = box_set_attr,
    .fn_draw     = box_draw,
};
