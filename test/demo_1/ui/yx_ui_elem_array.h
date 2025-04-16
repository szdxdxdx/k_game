#ifndef YX_UI_ELEM_ARRAY_H
#define YX_UI_ELEM_ARRAY_H

struct yx_ui_elem;

#define k_array_tmpl_struct_name      yx_ui_elem_array
#define k_array_tmpl_elem_type        struct yx_ui_elem *
#define k_array_tmpl_define_struct
#define k_array_tmpl_elem_pass_by_val
#include "k_array_tmpl.h"

#endif
