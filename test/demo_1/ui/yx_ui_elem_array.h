#ifndef YX_UI_ELEM_ARRAY_H
#define YX_UI_ELEM_ARRAY_H

struct yx_ui_elem;

#define k_tmpl_array_struct_name yx_ui_elem_array
#define k_tmpl_array_elem_type   struct yx_ui_elem *
#define k_tmpl_array_define_struct
#include "k_array_tmpl.h"

#endif
