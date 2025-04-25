#include "../llk_ui_ext.h"

/* 作为 UI 元素树的根节点，元素的宽高与 UI 画布的宽高保持一致 */
struct llk_ui_elem_root {
    struct llk_ui_elem super;
};

struct llk_ui_elem_type llk__ui_elem_root = {
    .type_name         = "root",
    .elem_struct_size  = sizeof(struct llk_ui_elem_root),
    .fn_init           = NULL,
    .fn_fini           = NULL,
    .fn_set_attr       = NULL,
    .fn_after_layout   = NULL,
    .fn_hit_test       = NULL,
    .fn_dispatch_event = NULL,
    .fn_draw           = NULL,
};
