
#include "./k_webui_internal.h"

int k__webui_exec_js(const char *js) {
    assert(NULL != js);

    webui_run(k__webui.window, js);
    return 0;
}

int k__webui_exec_js_fmt(const char *fmt, ...) {
    assert(NULL != fmt);

    char default_buf[512];

    char *buf = default_buf;
    size_t buf_capacity = sizeof(default_buf);

    va_list args;
    va_start(args, fmt);
    int str_len = k_vsnprintf(k__webui_fmt, buf, buf_capacity, fmt, args);
    va_end(args);

    if (-1 == str_len)
        return -1;

    if (str_len < buf_capacity) {
        return k__webui_exec_js(buf);
    }

    buf_capacity = str_len + 1;
    buf = k__webui_malloc(buf_capacity);
    if (NULL == buf)
        return -1;

    va_start(args, fmt);
    k_vsnprintf(k__webui_fmt, buf, buf_capacity, fmt, args);
    va_end(args);

    int result = k__webui_exec_js(buf);

    k__webui_free(buf);

    return result;
}
