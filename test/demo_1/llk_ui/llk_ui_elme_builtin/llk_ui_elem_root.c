#include "../llk_ui_ext.h"

struct llk_ui_elem_type llk__ui_elem_root = {
    .type_name         = "root",
    .data_size         = 0,
    .fn_init           = NULL,
    .fn_fini           = NULL,
    .fn_set_attr       = NULL,
    .fn_after_layout   = NULL,
    .fn_hit_test       = NULL,
    .fn_dispatch_event = NULL,
    .fn_draw           = NULL,
};
