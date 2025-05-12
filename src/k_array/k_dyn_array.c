
#include "k_dyn_array.h"

/* k_array_tmpl.h 头文件支持以下参数：
 *
 * - K_TMPL_ARRAY_STRUCT_NAME      必须定义此宏，用于指定数组结构体的名字
 * - K_TMPL_ARRAY_ELEM_TYPE        必须定义此宏，指定数组中元素的类型
 * - K_TMPL_ARRAY_DEFINE_STRUCT    若定义此宏，则定义结构体，否则只声明
 * - K_TMPL_ARRAY_DEFINE_FUNCTION  若定义此宏，则定义函数，否则只声明
 * - K_TMPL_ARRAY_FN_MALLOC        若定义此宏，则使用指定的内存分配函数，否则使用 `malloc()`
 * - K_TMPL_ARRAY_FN_FREE          若定义此宏，则使用指定的内存释放函数，否则使用 `free()`
 * - K_TMPL_ARRAY_PASS_ELEM_BY_PTR 若定义此宏，则操作单个元素（例如：插入单个元素）时传递指针，否则传值
 * - K_TMPL_ARRAY_STATIC_FUNCTION  若定义此宏，则用 static 修饰函数
 */

#define K_TMPL_ARRAY_STRUCT_NAME k_int_array  /* 定义数组的结构体名字 */
#define K_TMPL_ARRAY_ELEM_TYPE   int          /* 定义数组的结构体名字 */
// #define K_TMPL_ARRAY_DEFINE_STRUCT   此处不需要生成结构体的定义，在别处已经生成过该定义过了
#define K_TMPL_ARRAY_DEFINE_FUNCTION /* 此处需生成函数实现 */
#include "./k_array_tmpl.h" /* 引入头文件，生成代码 */

#define K_TMPL_ARRAY_STRUCT_NAME k_float_array
#define K_TMPL_ARRAY_ELEM_TYPE   float
#define K_TMPL_ARRAY_DEFINE_FUNCTION
#include "./k_array_tmpl.h"

#define K_TMPL_ARRAY_STRUCT_NAME k_ptr_array
#define K_TMPL_ARRAY_ELEM_TYPE   void *
#define K_TMPL_ARRAY_DEFINE_FUNCTION
#include "./k_array_tmpl.h"
