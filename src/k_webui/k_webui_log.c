#include <stdio.h>

#include "./k_webui_context.h"

enum k_webui_log_level {
    K__WEBUI_LOG_INFO,
    K__WEBUI_LOG_WARN,
    K__WEBUI_LOG_ERROR,
};

static int k__webui_log(enum k_webui_log_level level, const char *fmt, va_list args) {

    if (NULL == fmt || '\0' == fmt[0])
        return -1;

    const char *js_fn;
    switch (level) {
        case K__WEBUI_LOG_INFO:  js_fn = "kWebui.logInfo(%'s)";  break;
        case K__WEBUI_LOG_WARN:  js_fn = "kWebui.logWarn(%'s)";  break;
        case K__WEBUI_LOG_ERROR: js_fn = "kWebui.logError(%'s)"; break;
    }

    char default_buf[512];
    char *msg = default_buf;

    va_list args_copy;
    va_copy(args_copy, args);
    int str_len = vsnprintf(msg, sizeof(default_buf), fmt, args_copy);
    va_end(args_copy);

    if (str_len < 0)
        return -1;

    if (sizeof(default_buf) <= str_len) {
        msg = k__webui_mem_alloc(str_len + 1);
        if (NULL == msg)
            return -1;

        vsprintf(msg, fmt, args);
    }

    int r = k__webui_exec_js_fmt(js_fn, msg);

    if (msg != default_buf) {
        k__webui_mem_free(msg);
    }

    return r;
}

void k_webui_log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    k__webui_log(K__WEBUI_LOG_INFO, fmt, args);
    va_end(args);
}

void k_webui_log_warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    k__webui_log(K__WEBUI_LOG_WARN, fmt, args);
    va_end(args);
}

void k_webui_log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    k__webui_log(K__WEBUI_LOG_ERROR, fmt, args);
    va_end(args);
}
