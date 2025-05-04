
#include "k_dyn_array.h"

#define K_TMPL_ARRAY_STRUCT_NAME k_int_array
#define K_TMPL_ARRAY_ELEM_TYPE   int
#define K_TMPL_ARRAY_DEFINE_FUNCTION
#include "./k_array_tmpl.h"

#define K_TMPL_ARRAY_STRUCT_NAME k_float_array
#define K_TMPL_ARRAY_ELEM_TYPE   float
#define K_TMPL_ARRAY_DEFINE_FUNCTION
#include "./k_array_tmpl.h"

#define K_TMPL_ARRAY_STRUCT_NAME k_ptr_array
#define K_TMPL_ARRAY_ELEM_TYPE   void *
#define K_TMPL_ARRAY_DEFINE_FUNCTION
#include "./k_array_tmpl.h"
