#ifndef K_DYN_ARRAY_H
#define K_DYN_ARRAY_H

#define k_tmpl_array_struct_name k_int_array
#define k_tmpl_array_elem_type   int
#define k_tmpl_array_define_struct
#include "./k_array_tmpl.h"

#define k_tmpl_array_struct_name k_float_array
#define k_tmpl_array_elem_type   float
#define k_tmpl_array_define_struct
#include "./k_array_tmpl.h"

#define k_tmpl_array_struct_name k_ptr_array
#define k_tmpl_array_elem_type   void *
#define k_tmpl_array_define_struct
#include "./k_array_tmpl.h"

#endif
