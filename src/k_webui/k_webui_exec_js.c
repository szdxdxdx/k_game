#include "./k_webui_mem_alloc.h"
#include "./k_webui_context.h"
#include "./k_webui_k_printf.h"

int k__webui_exec_js(const char *js) {
    assert(NULL != js && '\0' != *js);

    webui_run(k__webui.window, js);
    return 0;
}

int k__webui_exec_js_fmt(const char *fmt, ...) {

    char buf[512];
    char *js = buf;

    va_list args;
    va_start(args, fmt);
    int str_len = k__webui_vsnprintf(js, sizeof(buf), fmt, args);
    va_end(args);

    if (-1 == str_len)
        return -1;

    if (str_len < sizeof(buf)) {
        return k__webui_exec_js(js);
    }

    js = k__webui_mem_alloc(str_len + 1);
    if (NULL == js)
        return -1;

    va_start(args, fmt);
    k__webui_vsprintf(js, fmt, args);
    va_end(args);

    int result = k__webui_exec_js(js);

    k__webui_mem_free(js);

    return result;
}
