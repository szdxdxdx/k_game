#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "k_game/core/k_canvas.h"

#include "./yx_ui_elem_button.h"
#include "./yx_ui_utils.h"

static int yx__ui_button_set_attr(struct yx_ui_elem *elem, const char *key, const char *val) {
    struct yx_ui_button *button = (struct yx_ui_button *)elem;

    if (0 == strncmp(key, "x", 1)) {
        char *end;
        float val_f = strtof(val, &end);
        if (end == val)
            return -1;

        button->x = val_f;
    }
    else if (0 == strncmp(key, "y", 1)) {
        char *end;
        float val_f = strtof(val, &end);
        if (end == val)
            return -1;

        button->y = val_f;
    }
    else if (0 == strncmp(key, "w", 1)) {
        char *end;
        float val_f = strtof(val, &end);
        if (end == val)
            return -1;

        button->w = val_f;
    }
    else if (0 == strncmp(key, "h", 1)) {
        char *end;
        float val_f = strtof(val, &end);
        if (end == val)
            return -1;

        button->h = val_f;
    }
    else if (0 == strncmp(key, "background_color", 16)) {

        uint32_t color;
        if ( ! yx__ui_parse_hex_color(val, &color))
            return -1;

        button->background_color = color;
        return 0;
    }

    return -1;
}

static void yx__ui_button_draw(struct yx_ui_elem *elem) {
    struct yx_ui_button *button = (struct yx_ui_button *)elem;

    k_canvas_set_draw_color_rgba(button->background_color);
    k_canvas_fill_rect(button->x, button->y, button->w, button->h);
}

static void yx__ui_button_destruct(struct yx_ui_elem *elem) {
    yx__ui_elem_destruct(elem);
}

static struct yx_ui_elem *yx__ui_button_construct(struct yx_ui_button *button, struct yx_ui_context *ui) {

    if (0 != yx__ui_elem_construct(&button->super, ui))
        return NULL;

    button->x = 10.0f;
    button->y = 10.0f;
    button->w = 100.0f;
    button->h = 50.0f;
    button->background_color = 0xff0099ff;

    struct yx_ui_elem_v_tbl *v_tbl = button->super.v_tbl;
    v_tbl->fn_destruct = yx__ui_button_destruct;
    v_tbl->fn_draw     = yx__ui_button_draw;
    v_tbl->fn_set_attr = yx__ui_button_set_attr;

    return &button->super;
}
