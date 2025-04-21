#include "k_game/core/k_canvas.h"

#include "./llk_ui_elem_type_builtin.h"
#include "llk_ui_elem_type_register.h"

static void llk__ui_elem_box_draw(struct llk_ui_elem *elem) {

    k_canvas_set_draw_color_rgba(elem->background_color);
    k_canvas_ui_fill_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);

    k_canvas_set_draw_color_rgba(elem->border_color);
    k_canvas_ui_draw_rect(elem->x, elem->y, elem->w.computed_val, elem->h.computed_val);
}

struct llk_ui_elem_type_config llk__ui_elem_box_config = {
    .type_name = "box",
    .data_size = sizeof(struct llk_ui_elem),
    .fn_init   = NULL,
    .fn_fini   = NULL,
    .fn_draw   = llk__ui_elem_box_draw,
};
