#include "./llk_ui_ext.h"
#include "./llk_ui_elem_type_builtin.h"

struct llk_ui_elem_type llk__ui_elem_root = {
    .type_name         = "root",
    .data_size         = 0,
    .fn_init           = NULL,
    .fn_fini           = NULL,
    .fn_set_attr       = NULL,
    .fn_draw           = NULL,
    .fn_dispatch_event = NULL,
};
