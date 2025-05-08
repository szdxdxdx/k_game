#include "k_game/core/k_canvas.h"

#include "../llk_ui_ext.h"

static int llk__ui_elem_box_init(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = (struct llk_ui_elem_box *)elem;

    box->background_color.unit= LLK_UI_UNIT_RGBA;
    box->background_color.specified_val = 0xffffff99;
    box->background_color.computed_val = 0xffffff99;
    box->background_color_hovered.unit = LLK_UI_UNIT_NO_VAL;
    box->background_color_pressed.unit = LLK_UI_UNIT_NO_VAL;

    box->border_color.unit = LLK_UI_UNIT_RGBA;
    box->border_color.specified_val = 0x00000099;
    box->border_color.computed_val = 0x00000099;
    box->border_color_hovered.unit = LLK_UI_UNIT_NO_VAL;
    box->border_color_pressed.unit = LLK_UI_UNIT_NO_VAL;

    box->fn_on_click = NULL;
    box->fn_on_draw  = NULL;

    return 0;
}

static int llk__ui_elem_box_set_color(struct llk_ui_u32 *set_color, const char *val) {

    uint32_t u32_val;
    enum llk_ui_unit unit;
    if (0 != llk__ui_parse_color_val(val, &u32_val, &unit))
        return -1;

    if (LLK_UI_UNIT_RGBA != unit)
        return -1;

    set_color->specified_val = u32_val;
    set_color->unit = unit;
    set_color->computed_val = u32_val;
    return 0;
}

static int llk__ui_elem_box_set_attr_fn_callback(struct llk_ui_context *ui, void *set_fn, const char *val) {

    if (val == NULL || 0 == strcmp(val, "null")) {
        *(void **)set_fn = NULL;
        return 0;
    }

    void *fn = llk__ui_get_callback(ui, val);
    if (NULL == fn)
        return -1;

    *(void **)set_fn = fn;
    return 0;
}

static int llk__ui_elem_box_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {
    struct llk_ui_elem_box *box = (struct llk_ui_elem_box *)elem;

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
        return llk__ui_elem_box_set_color(&box->border_color_pressed, val);

    if (0 == strcmp(key, "on-click"))
        return llk__ui_elem_box_set_attr_fn_callback(elem->ui, &box->fn_on_click, val);
    if (0 == strcmp(key, "on-draw"))
        return llk__ui_elem_box_set_attr_fn_callback(elem->ui, &box->fn_on_draw, val);

    return llk__ui_elem_set_attr_default(elem, key, val);
}

static void llk__ui_elem_box_dispatch_event(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = (struct llk_ui_elem_box *)elem;

    if (llk_ui_elem_is_clicked(elem)) {

        if (NULL != box->fn_on_click) {
            box->fn_on_click(elem);
        }
    }
}

static void llk__ui_elem_box_draw(struct llk_ui_elem *elem) {
    struct llk_ui_elem_box *box = (struct llk_ui_elem_box *)elem;

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
        k_canvas_set_draw_color(background_color);
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
        k_canvas_set_draw_color(border_color);
        k_canvas_ui_draw_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
    }

    if (NULL != box->fn_on_draw) {
        box->fn_on_draw(elem);
        return;
    }
}

struct llk_ui_elem_type llk__ui_elem_box = {
    .type_name         = "box",
    .elem_struct_size  = sizeof(struct llk_ui_elem_box),
    .fn_init           = llk__ui_elem_box_init,
    .fn_fini           = NULL,
    .fn_set_attr       = llk__ui_elem_box_set_attr,
    .fn_after_layout   = NULL,
    .fn_hit_test       = NULL,
    .fn_dispatch_event = llk__ui_elem_box_dispatch_event,
    .fn_draw           = llk__ui_elem_box_draw,
};
