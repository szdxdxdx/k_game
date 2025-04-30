#ifndef K__WEBUI_CONTEXT_H
#define K__WEBUI_CONTEXT_H

#include "webui.h"

#include "k_printf.h"

#include "k_webui.h"
#include "k_str_map.h"

/* region [k_printf] */

/* 自定义格式说明符：
 *
 * - `%'s`
 *   将 C 字符串转换输出为 js 字符串字面量，输出的字符串会被双引号包裹，
 *   且在所有需要转义的字符前（引号、回车等）添加反斜杠。
 */
k_printf_spec_print_fn k__webui_fmt(const char **str);

/* endregion */

/* region [mem_alloc] */

void *k__webui_malloc(size_t size);

void  k__webui_free(void *p);

char *k__webui_strdup(const char *str, size_t len);

/* endregion */

/* region [context] */

struct k_webui_context;

struct k_webui_context {

    /* 标记是否已完成初始化 */
    unsigned int is_inited;

    /* webui 窗口 */
    size_t window;
};

extern struct k_webui_context k__webui;

/* 判断 webui 窗口是否在运行 */
int k__webui_is_running(void);

/* endregion */

/* region [exec_js] */

int k__webui_exec_js(const char *js);

int k__webui_exec_js_fmt(const char *js, ...);

/* endregion */

/* region [binding] */

int k__webui_binding_init(void);

void k__webui_binding_fini(void);


/* endregion */

#endif
