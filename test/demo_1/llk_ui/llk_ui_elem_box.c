#include "k_game/core/k_canvas.h"

#include "./llk_ui_elem_type_builtin.h"
#include "./llk_ui_elem_type.h"
#include "./llk_ui_val_parser.h"

struct llk_ui_elem_box {

    uint32_t background_color;
    uint32_t background_color_hovered;
    uint32_t border_color;
};

static int llk__ui_elem_box_init(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = elem->data;

    box->background_color = 0x00000000;
    box->border_color     = 0x00000000;

    return 0;
}

static int llk__ui_elem_box_set_attr_background_color(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_box *box = elem->data;

    uint32_t u32_val;
    if (0 != llk__ui_parse_color_val(val, &u32_val))
        return -1;

    box->background_color = u32_val;
    return 0;
}

static int llk__ui_elem_box_set_attr_background_color_hovered(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_box *box = elem->data;

    uint32_t u32_val;
    if (0 != llk__ui_parse_color_val(val, &u32_val))
        return -1;

    box->background_color_hovered = u32_val;
    return 0;
}

static int llk__ui_elem_box_set_attr_border_color(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_box *box = elem->data;

    uint32_t u32_val;
    if (0 != llk__ui_parse_color_val(val, &u32_val))
        return -1;

    box->border_color = u32_val;
    return 0;
}

static int llk__ui_elem_box_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {

    if (0 == strcmp(key, "background-color"))
        return llk__ui_elem_box_set_attr_background_color(elem, val);
    if (0 == strcmp(key, "background-color.hover"))
        return llk__ui_elem_box_set_attr_background_color_hovered(elem, val);
    if (0 == strcmp(key, "border-color"))
        return llk__ui_elem_box_set_attr_border_color(elem, val);

    return llk__ui_elem_set_attr_default(elem, key, val);
}

static void llk__ui_elem_box_draw(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = elem->data;

    if (elem->is_hovered) {
        k_canvas_set_draw_color_rgba(box->background_color_hovered);
        k_canvas_ui_fill_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
    } else {
        k_canvas_set_draw_color_rgba(box->background_color);
        k_canvas_ui_fill_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
    }

    k_canvas_set_draw_color_rgba(box->border_color);
    k_canvas_ui_draw_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
}

struct llk_ui_elem_type_config llk__ui_elem_box_config = {
    .type_name   = "box",
    .data_size   = sizeof(struct llk_ui_elem_box),
    .fn_init     = llk__ui_elem_box_init,
    .fn_fini     = NULL,
    .fn_set_attr = llk__ui_elem_box_set_attr,
    .fn_draw     = llk__ui_elem_box_draw,
};
