#include "k_webui.h"
#include "./k_webui_mem_alloc.h"
#include "./k_webui_context.h"
#include "./k_webui_exec_js.h"

int k_webui_puts(const char *s) {

    if (NULL == s || '\0' == s[0])
        return -1;

    return k__webui_exec_js_fmt("kWebui.logInfo(%s)", s);
}

int k_webui_printf(const char *fmt, ...) {

    if (NULL == fmt || '\0' == fmt[0])
        return -1;

    char default_buf[512];

    va_list args;
    va_start(args, fmt);
    int str_len = vsnprintf(default_buf, sizeof(default_buf), fmt, args);
    va_end(args);

    if (str_len < 0)
        return -1;

    if (str_len < sizeof(default_buf)) {
        return k_webui_puts(default_buf);
    }

    char *alloc_buf = k__webui_mem_alloc(str_len + 1);
    if (NULL == alloc_buf)
        return -1;

    va_start(args, fmt);
    vsprintf(alloc_buf, fmt, args);
    va_end(args);

    int r = k_webui_puts(alloc_buf);

    k__webui_mem_free(alloc_buf);

    return r;
}
