#include <stdlib.h>
#include <math.h>

#include "k_game/core/k_canvas.h"

#include "../llk_ui_ext.h"

/* 滑块最大宽 12 像素 */
#define SLIDER_THUMB_MAX_W 12

static int llk__ui_elem_slider_init(struct llk_ui_elem *elem) {
    struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;

    slider->track_color.unit= LLK_UI_UNIT_RGBA;
    slider->track_color.specified_val= 0xee000099;
    slider->track_color.computed_val= 0xee000099;
    slider->track_color_hovered.unit = LLK_UI_UNIT_NO_VAL;
    slider->track_color_pressed.unit = LLK_UI_UNIT_NO_VAL;

    slider->thumb_color.unit= LLK_UI_UNIT_RGBA;
    slider->thumb_color.specified_val= 0xff6600dd;
    slider->thumb_color.computed_val = 0xff6600dd;
    slider->thumb_color_hovered.unit = LLK_UI_UNIT_RGBA;
    slider->thumb_color_hovered.specified_val = 0xff6600ff;
    slider->thumb_color_hovered.computed_val  = 0xff6600ff;
    slider->thumb_color_pressed.unit = LLK_UI_UNIT_NO_VAL;

    slider->border_color.unit = LLK_UI_UNIT_RGBA;
    slider->border_color.specified_val = 0xff6600ff;
    slider->border_color.computed_val  = 0xff6600ff;
    slider->border_color_hovered.unit = LLK_UI_UNIT_RGBA;
    slider->border_color_hovered.specified_val = 0xffffffff;
    slider->border_color_hovered.computed_val  = 0xffffffff;
    slider->border_color_pressed.unit = LLK_UI_UNIT_NO_VAL;

    slider->min     = 0.0f;
    slider->max     = 1.0f;
    slider->step    = 0.01f;

    slider->val     = 0.0f;
    slider->old_val = 0.0f;

    slider->is_active = 0;
    slider->is_changed = 0;

    slider->fn_on_change = NULL;

    return 0;
}

static int llk__ui_elem_slider_set_color(struct llk_ui_u32 *set_color, const char *val) {

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

static int llk__ui_elem_slider_set_attr_fn_callback(struct llk_ui_context *ui, void *set_fn, const char *val) {

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

static int llk__ui_elem_slider_set_attr_min(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;

    char *end;
    float min = strtof(val, &end);
    if (end == val || '\0' != *end)
        return -1;

    slider->min = min;

    if (slider->max < slider->min) {
        slider->max = slider->min;
    }
    if (slider->val < slider->min) {
        slider->val = slider->min;
    }

    return 0;
}

static int llk__ui_elem_slider_set_attr_max(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;

    char *end;
    float max = strtof(val, &end);
    if (end == val || '\0' != *end)
        return -1;

    slider->max = max;

    if (slider->min > slider->max) {
        slider->min = slider->max;
    }
    if (slider->val > slider->max) {
        slider->val = slider->max;
    }

    return 0;
}

static int llk__ui_elem_slider_set_attr_val(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;

    char *end;
    float val_ = strtof(val, &end);
    if (end == val || '\0' != *end)
        return -1;

    if (val_ < slider->min || slider->max < val_)
        return -1;

    slider->val = val_;
    return 0;
}

static int llk__ui_elem_slider_set_attr_step(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;

    char *end;
    float step = strtof(val, &end);
    if (end == val || '\0' != *end)
        return -1;

    if (step <= 0.0f)
        return -1;

    slider->step = step;

    return 0;
}

static int llk__ui_elem_slider_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {
    struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;

    if (0 == strcmp(key, "track-color"))
        return llk__ui_elem_slider_set_color(&slider->track_color, val);
    if (0 == strcmp(key, "track-color.hovered"))
        return llk__ui_elem_slider_set_color(&slider->track_color_hovered, val);
    if (0 == strcmp(key, "track-color.pressed"))
        return llk__ui_elem_slider_set_color(&slider->track_color_pressed, val);

    if (0 == strcmp(key, "border-color"))
        return llk__ui_elem_slider_set_color(&slider->border_color, val);
    if (0 == strcmp(key, "border-color.hovered"))
        return llk__ui_elem_slider_set_color(&slider->border_color_hovered, val);
    if (0 == strcmp(key, "border-color.pressed"))
        return llk__ui_elem_slider_set_color(&slider->border_color_pressed, val);

    if (0 == strcmp(key, "thumb-color"))
        return llk__ui_elem_slider_set_color(&slider->thumb_color, val);
    if (0 == strcmp(key, "thumb-color.hovered"))
        return llk__ui_elem_slider_set_color(&slider->thumb_color_hovered, val);
    if (0 == strcmp(key, "thumb-color.pressed"))
        return llk__ui_elem_slider_set_color(&slider->thumb_color_pressed, val);

    if (0 == strcmp(key, "min"))
        return llk__ui_elem_slider_set_attr_min(elem, val);
    if (0 == strcmp(key, "max"))
        return llk__ui_elem_slider_set_attr_max(elem, val);
    if (0 == strcmp(key, "val"))
        return llk__ui_elem_slider_set_attr_val(elem, val);
    if (0 == strcmp(key, "step"))
        return llk__ui_elem_slider_set_attr_step(elem, val);

    if (0 == strcmp(key, "on-change"))
        return llk__ui_elem_slider_set_attr_fn_callback(elem->ui, &slider->fn_on_change, val);

    return llk__ui_elem_set_attr_default(elem, key, val);
}

static float clamp(float min, float val, float max) {
    return fmaxf(min, fminf(val, max));
}

static void llk__ui_elem_slider_hit_test(struct llk_ui_elem *elem) {
    struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;
    struct llk_ui_context *ui = elem->ui;

    if (slider->is_active) {
        if ( ! (ui->mouse_button_down || ui->mouse_button_pressed)) {
            slider->is_active = 0;
            return;
        }
    } else {
        if (elem->is_hovered && ui->mouse_button_down) {
            slider->is_active = 1;
        } else {
            return;
        }
    }

    float thumb_w = fminf(elem->w.computed_val, SLIDER_THUMB_MAX_W);
    float thumb_x = clamp(elem->x, ui->mouse_x - thumb_w / 2, elem->x + elem->w.computed_val - thumb_w);

    float new_val = slider->min + roundf((slider->max - slider->min) * (thumb_x - elem->x) / (elem->w.computed_val - thumb_w) / slider->step) * slider->step;
    if (new_val != slider->val) {
        slider->old_val = slider->val;
        slider->val = new_val;
        slider->is_changed = 1;
    }
}

static void llk__ui_elem_slider_dispatch_event(struct llk_ui_elem *elem) {
    struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;

    if (slider->is_changed) {
        slider->is_changed = 0;

        if (NULL != slider->fn_on_change) {
            slider->fn_on_change(elem, slider->old_val, slider->val);
        }
    }
}

static void llk__ui_elem_slider_draw(struct llk_ui_elem *elem) {
    struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;

    int pressed = llk_ui_elem_is_pressed(elem);
    int hovered = llk_ui_elem_is_hovered(elem);

    uint32_t track_color = 0x00000000;
    if (pressed && llk__ui_is_val_specified(slider->track_color_pressed)) {
        track_color = slider->track_color_pressed.computed_val;
    }
    else if (hovered && llk__ui_is_val_specified(slider->track_color_hovered)) {
        track_color = slider->track_color_hovered.computed_val;
    }
    else if (llk__ui_is_val_specified(slider->track_color)) {
        track_color = slider->track_color.computed_val;
    }

    uint32_t thumb_color = 0x00000000;
    if (pressed && llk__ui_is_val_specified(slider->thumb_color_pressed)) {
        thumb_color = slider->thumb_color_pressed.computed_val;
    }
    else if (hovered && llk__ui_is_val_specified(slider->thumb_color_hovered)) {
        thumb_color = slider->thumb_color_hovered.computed_val;
    }
    else if (llk__ui_is_val_specified(slider->thumb_color)) {
        thumb_color = slider->thumb_color.computed_val;
    }

    uint32_t border_color = 0x00000000;
    if (pressed && llk__ui_is_val_specified(slider->border_color_pressed)) {
        border_color = slider->border_color_pressed.computed_val;
    }
    else if (hovered && llk__ui_is_val_specified(slider->border_color_hovered)) {
        border_color = slider->border_color_hovered.computed_val;
    }
    else if (llk__ui_is_val_specified(slider->border_color)) {
        border_color = slider->border_color.computed_val;
    }

    float track_h = elem->h.computed_val;
    float track_y = elem->y;

    float thumb_w = fminf(elem->w.computed_val, SLIDER_THUMB_MAX_W);
    float thumb_x = elem->x + (slider->val - slider->min) * (elem->w.computed_val - thumb_w) / (slider->max - slider->min);
    if (thumb_x > elem->x + elem->w.computed_val - thumb_w) {
        thumb_x = elem->x + elem->w.computed_val - thumb_w;
    }

    float track_left_x  = elem->x;
    float track_left_w  = thumb_x - track_left_x;
    float track_right_x = thumb_x + thumb_w;
    float track_right_w = elem->w.computed_val - track_left_w - thumb_w;

    if (0x00000000 != track_color) {
        k_canvas_set_draw_color(track_color);
        k_canvas_ui_fill_rect(track_left_x, track_y, track_left_w, track_h);
        k_canvas_ui_fill_rect(track_right_x, track_y, track_right_w, track_h);
    }
    if (0x00000000 != thumb_color) {
        k_canvas_set_draw_color(thumb_color);
        k_canvas_ui_fill_rect(thumb_x, track_y, thumb_w, track_h);
    }
    if (0x00000000 != border_color) {
        k_canvas_set_draw_color(border_color);
        k_canvas_ui_draw_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
        k_canvas_ui_draw_rect(thumb_x, track_y, thumb_w, track_h);
    }
}

struct llk_ui_elem_type llk__ui_elem_slider = {
    .type_name         = "slider",
    .elem_struct_size  = sizeof(struct llk_ui_elem_slider),
    .on_create         = llk__ui_elem_slider_init,
    .on_destroy        = NULL,
    .fn_set_attr       = llk__ui_elem_slider_set_attr,
    .fn_after_layout   = NULL,
    .fn_hit_test       = llk__ui_elem_slider_hit_test,
    .fn_dispatch_event = llk__ui_elem_slider_dispatch_event,
    .fn_draw           = llk__ui_elem_slider_draw,
};
