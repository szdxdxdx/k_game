#include "k_game/core/k_canvas.h"

#include "./llk_ui_ext.h"
#include "./llk_ui_elem_type_builtin.h"

struct llk_ui_elem_box {

    struct llk_ui_u32 background_color;
    struct llk_ui_u32 background_color_hovered;
    struct llk_ui_u32 background_color_pressed;

    struct llk_ui_u32 border_color;
    struct llk_ui_u32 border_color_hovered;
    struct llk_ui_u32 border_color_pressed;

    llk_ui_callback_fn fn_on_click;
};

static int llk__ui_elem_box_init(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = elem->data;

    box->background_color.unit= LLK_UI_UNIT_NO_VAL;
    box->background_color_hovered.unit = LLK_UI_UNIT_NO_VAL;
    box->background_color_pressed.unit = LLK_UI_UNIT_NO_VAL;

    box->border_color.unit = LLK_UI_UNIT_NO_VAL;
    box->border_color_hovered.unit = LLK_UI_UNIT_NO_VAL;
    box->border_color_pressed.unit = LLK_UI_UNIT_NO_VAL;

    box->fn_on_click = NULL;

    return 0;
}

static int llk__ui_elem_box_set_color(struct llk_ui_u32 *elem_val, const char *val) {

    uint32_t u32_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_color_val(val, &u32_val, &unit))
        return -1;

    if (LLK_UI_UNIT_RGBA != unit)
        return -1;

    elem_val->specified_val = u32_val;
    elem_val->unit = unit;
    elem_val->computed_val = u32_val;
    return 0;
}

static int llk__ui_elem_box_set_attr_on_click(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_box *box = elem->data;

    if (val == NULL || 0 == strcmp(val, "null")) {
        box->fn_on_click = NULL;
        return 0;
    }

    llk_ui_callback_fn fn = llk__ui_get_callback(elem->ui, val);
    if (NULL == fn)
        return -1;

    box->fn_on_click = fn;
    return 0;
}

static int llk__ui_elem_box_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {
    struct llk_ui_elem_box *box = elem->data;

    if (0 == strcmp(key, "background-color"))
        return llk__ui_elem_box_set_color(&box->background_color, val);
    if (0 == strcmp(key, "background-color.hovered"))
        return llk__ui_elem_box_set_color(&box->background_color_hovered, val);
    if (0 == strcmp(key, "background-color.pressed"))
        return llk__ui_elem_box_set_color(&box->background_color_pressed, val);

    if (0 == strcmp(key, "border-color"))
        return llk__ui_elem_box_set_color(&box->border_color, val);
    if (0 == strcmp(key, "border-color.hovered"))
        return llk__ui_elem_box_set_color(&box->border_color_hovered, val);
    if (0 == strcmp(key, "border-color.pressed"))
        return llk__ui_elem_box_set_color(&box->background_color_pressed, val);

    if (0 == strcmp(key, "on-click"))
        return llk__ui_elem_box_set_attr_on_click(elem, val);

    return llk__ui_elem_set_attr_default(elem, key, val);
}

static void llk__ui_elem_box_draw(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = elem->data;

    int pressed = llk_ui_elem_is_pressed(elem);
    int hovered = llk_ui_elem_is_hovered(elem);

    uint32_t background_color = 0x00000000;
    if (pressed && llk__ui_is_val_specified(box->background_color_pressed)) {
        background_color = box->background_color_pressed.computed_val;
    }
    else if (hovered && llk__ui_is_val_specified(box->background_color_hovered)) {
        background_color = box->background_color_hovered.computed_val;
    }
    else if (llk__ui_is_val_specified(box->background_color)) {
        background_color = box->background_color.computed_val;
    }
    if (0x00000000 != background_color) {
        k_canvas_set_draw_color_rgba(background_color);
        k_canvas_ui_fill_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
    }

    uint32_t border_color = 0x00000000;
    if (pressed && llk__ui_is_val_specified(box->border_color_pressed)) {
        border_color = box->border_color_pressed.computed_val;
    }
    else if (hovered && llk__ui_is_val_specified(box->border_color_hovered)) {
        border_color = box->border_color_hovered.computed_val;
    }
    else if (llk__ui_is_val_specified(box->border_color)) {
        border_color = box->border_color.computed_val;
    }
    if (0x00000000 != border_color) {
        k_canvas_set_draw_color_rgba(border_color);
        k_canvas_ui_draw_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
    }
}

static void llk__ui_elem_box_dispatch_event(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = elem->data;

    if (elem->is_hovered && elem->ui->mouse_button_pressed) {

        if (NULL != box->fn_on_click) {
            box->fn_on_click(elem);
        }
    }
}

struct llk_ui_elem_type llk__ui_elem_box = {
    .type_name         = "box",
    .data_size         = sizeof(struct llk_ui_elem_box),
    .fn_init           = llk__ui_elem_box_init,
    .fn_fini           = NULL,
    .fn_set_attr       = llk__ui_elem_box_set_attr,
    .fn_draw           = llk__ui_elem_box_draw,
    .fn_dispatch_event = llk__ui_elem_box_dispatch_event,
};
