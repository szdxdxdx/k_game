#ifndef K__WEBUI_CONTEXT_H
#define K__WEBUI_CONTEXT_H

#include "webui.h"

#include "k_webui.h"
#include "k_str_map.h"

/* region [printf] */

/* 自定义格式说明符：
 *
 * - `%'s`
 *   将 C 字符串转换输出为 js 字符串字面量，输出的字符串会被双引号包裹，
 *   且在所有需要转义的字符前（引号、回车等）添加反斜杠。
 */
int k__webui_sprintf  (char *buf, const char *fmt, ...);
int k__webui_vsprintf (char *buf, const char *fmt, va_list args);
int k__webui_snprintf (char *buf, size_t n, const char *fmt, ...);
int k__webui_vsnprintf(char *buf, size_t n, const char *fmt, va_list args);

/* endregion */

/* region [mem_alloc] */

void *k__webui_mem_alloc(size_t size);

void  k__webui_mem_free(void *p);

char *k__webui_strdup(const char *str, size_t len);

/* endregion */

/* region [context] */

struct k_webui_context;

struct k_webui_context {

    /* 标记是否已完成初始化 */
    unsigned int is_inited;

    /* webui 窗口 */
    size_t window;

    struct k_str_map bindings;
};

extern struct k_webui_context k__webui;

/* 判断 webui 窗口是否在运行 */
int k__webui_is_running(void);

/* endregion */

/* region [exec_js] */

int k__webui_exec_js(const char *js, ...);

/* endregion */

/* region [binding] */

struct k_webui_binding {

    void *data;

    void (*fn_unbind)(void *data);
};

int k__webui_binding_init(void);

void k__webui_binding_fini(void);

/* endregion */

#endif
