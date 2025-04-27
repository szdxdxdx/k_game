
#include "./k_webui_context.h"

static int exec_js(const char *js) {
    webui_run(k__webui.window, js);
    return 0;
}

int k__webui_exec_js(const char *fmt, ...) {
    assert(NULL != fmt && '\0' != fmt[0]);

    char default_buf[512];

    va_list args;
    va_start(args, fmt);
    int str_len = k__webui_vsnprintf(default_buf, sizeof(default_buf), fmt, args);
    va_end(args);

    if (-1 == str_len)
        return -1;

    if (str_len < sizeof(default_buf)) {
        return exec_js(default_buf);
    }

    char *alloc_buf = k__webui_mem_alloc(str_len + 1);
    if (NULL == alloc_buf)
        return -1;

    va_start(args, fmt);
    k__webui_vsprintf(alloc_buf, fmt, args);
    va_end(args);

    int result = exec_js(alloc_buf);

    k__webui_mem_free(alloc_buf);

    return result;
}
