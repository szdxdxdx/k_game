#include "./llk_ui_elem.h"

enum llk_ui_linear_layout_direction {
    LLK__DIRECTION_X,
    LLK__DIRECTION_Y,
    LLK__DIRECTION_X_REVERSE,
    LLK__DIRECTION_Y_REVERSE,
};

enum llk_ui_linear_layout_align {
    LLK__ALIGN_LEFT,
    LLK__ALIGN_RIGHT,
    LLK__ALIGN_TOP,
    LLK__ALIGN_BOTTOM,
    LLK__ALIGN_CENTER,
};

struct llk_ui_linear_layout {
    struct llk_ui_elem super;

    enum llk_ui_linear_layout_direction direction;
    enum llk_ui_linear_layout_align x_align;
    enum llk_ui_linear_layout_align y_align;
};

struct llk_ui_elem *llk_ui_create_linear_layout(struct llk_ui_context *ui) {

    struct llk_ui_linear_layout *layout = malloc(sizeof(struct llk_ui_linear_layout));
    if (NULL == layout)
        return NULL;

    struct llk_ui_elem *super = &layout->super;
    llk_ui_construct_elem(super, ui);
    super->elem_type_name = "linear_layout";

    layout->direction = LLK__DIRECTION_Y;
    layout->x_align = LLK__ALIGN_LEFT;
    layout->y_align = LLK__ALIGN_TOP;

    return &layout->super;
}
