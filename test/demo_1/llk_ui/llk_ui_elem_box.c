#include "k_game/core/k_canvas.h"

#include "./llk_ui_context.h"
#include "./llk_ui_elem_type_builtin.h"
#include "./llk_ui_elem_type.h"
#include "./llk_ui_val_parser.h"

struct llk_ui_elem_box {

    struct llk_ui_u32 background_color;
    struct llk_ui_u32 background_color_hovered;
    struct llk_ui_u32 background_color_activated;
    struct llk_ui_u32 border_color;

    llk_ui_callback_fn fn_on_click;
};

static int llk__ui_elem_box_init(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = elem->data;

    box->background_color.unit= LLK_UI_UNIT_NO_VAL;
    box->background_color.computed_val = 0x00000000;

    box->background_color_hovered.unit = LLK_UI_UNIT_NO_VAL;
    box->background_color_hovered.computed_val = 0x00000000;

    box->background_color_activated.unit = LLK_UI_UNIT_NO_VAL;
    box->background_color_activated.computed_val = 0x00000000;

    box->border_color.unit = LLK_UI_UNIT_NO_VAL;
    box->border_color.computed_val = 0x00000000;

    box->fn_on_click = NULL;

    return 0;
}

static int llk__ui_elem_box_set_attr_background_color(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_box *box = elem->data;

    uint32_t u32_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_color_val(val, &u32_val, &unit))
        return -1;

    if (LLK_UI_UNIT_RGBA != unit)
        return -1;

    box->background_color.specified_val = u32_val;
    box->background_color.unit = unit;
    box->background_color.computed_val = u32_val;
    return 0;
}

static int llk__ui_elem_box_set_attr_background_color_hovered(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_box *box = elem->data;

    uint32_t u32_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_color_val(val, &u32_val, &unit))
        return -1;

    if (LLK_UI_UNIT_RGBA != unit)
        return -1;

    box->background_color_hovered.specified_val = u32_val;
    box->background_color_hovered.unit = unit;
    box->background_color_hovered.computed_val = u32_val;
    return 0;
}

static int llk__ui_elem_box_set_attr_background_color_activated(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_box *box = elem->data;

    uint32_t u32_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_color_val(val, &u32_val, &unit))
        return -1;

    if (LLK_UI_UNIT_RGBA != unit)
        return -1;

    box->background_color_activated.specified_val = u32_val;
    box->background_color_activated.unit = unit;
    box->background_color_activated.computed_val = u32_val;
    return 0;
}

static int llk__ui_elem_box_set_attr_border_color(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_box *box = elem->data;

    uint32_t u32_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_color_val(val, &u32_val, &unit))
        return -1;

    if (LLK_UI_UNIT_RGBA != unit)
        return -1;

    box->border_color.specified_val = u32_val;
    box->border_color.unit = unit;
    box->border_color.computed_val = u32_val;
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

    if (0 == strcmp(key, "background-color"))
        return llk__ui_elem_box_set_attr_background_color(elem, val);
    if (0 == strcmp(key, "background-color.hover"))
        return llk__ui_elem_box_set_attr_background_color_hovered(elem, val);
    if (0 == strcmp(key, "background-color.active"))
        return llk__ui_elem_box_set_attr_background_color_activated(elem, val);
    if (0 == strcmp(key, "border-color"))
        return llk__ui_elem_box_set_attr_border_color(elem, val);
    if (0 == strcmp(key, "on-click"))
        return llk__ui_elem_box_set_attr_on_click(elem, val);

    return llk__ui_elem_set_attr_default(elem, key, val);
}

static void llk__ui_elem_box_draw(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = elem->data;

    uint32_t background_color = box->background_color.computed_val;

    if (elem->is_hovered && LLK_UI_UNIT_NO_VAL != box->background_color_hovered.unit) {
        background_color = box->background_color_hovered.computed_val;

        if (elem->ui->mouse_button_down && LLK_UI_UNIT_NO_VAL != box->background_color_activated.unit)
            background_color = box->background_color_activated.computed_val;
    }

    k_canvas_set_draw_color_rgba(background_color);
    k_canvas_ui_fill_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);

    k_canvas_set_draw_color_rgba(box->border_color.computed_val);
    k_canvas_ui_draw_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
}

static void llk__ui_elem_box_dispatch_event(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = elem->data;

    if (elem->is_hovered && elem->ui->mouse_button_pressed) {

        if (NULL != box->fn_on_click) {
            box->fn_on_click();
        }
    }
}

struct llk_ui_elem_type_config llk__ui_elem_box_config = {
    .type_name         = "box",
    .data_size         = sizeof(struct llk_ui_elem_box),
    .fn_init           = llk__ui_elem_box_init,
    .fn_fini           = NULL,
    .fn_set_attr       = llk__ui_elem_box_set_attr,
    .fn_draw           = llk__ui_elem_box_draw,
    .fn_dispatch_event = llk__ui_elem_box_dispatch_event,
};
