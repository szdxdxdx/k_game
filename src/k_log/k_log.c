#include <stdarg.h>
#include <stdio.h>

#include "k_log.h"

#if K_LOG_COLOR == 1
#define C_RESET   "\033[0m"
#define C_RED     "\033[91m"
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_BLUE    "\033[94m"
#define C_MAGENTA "\033[95m"
#define C_CYAN    "\033[36m"
#define C_WHITE   "\033[37m"
#else
#define C_RESET
#define C_RED
#define C_GREEN
#define C_YELLOW
#define C_BLUE
#define C_MAGENTA
#define C_CYAN
#define C_WHITE
#endif

static inline const char *level_str(int level) {

    switch(level) {
        case K_LOG_LEVEL_TRACE: return C_MAGENTA "T" C_RESET;
        case K_LOG_LEVEL_DEBUG: return C_MAGENTA "D" C_RESET;
        case K_LOG_LEVEL_INFO : return C_CYAN    "I" C_RESET;
        case K_LOG_LEVEL_WARN : return C_YELLOW  "W";
        case K_LOG_LEVEL_ERROR: return C_RED     "E";
        case K_LOG_LEVEL_FATAL: return C_RED     "F";
        default:                return C_CYAN    "I" C_RESET;
    }
}

/* region [v1] */

void log_v1(int level, const char *fmt, va_list args) {

    char buf[512];
    snprintf(buf, sizeof(buf), "%s %s\n"C_RESET, level_str(level), fmt);
    vfprintf(stdout, buf, args);
}

/* endregion */

/* region [v2] */

static inline int is_k_fn_name(const char *fn) {
    return fn[0] == 'k' && fn[1] == '_' && fn[2] != '_';
}

static inline int is_add_indent(const char *fmt) {
    return '>' == fmt[0] && ' ' == fmt[1];
}

static inline int is_sub_indent(const char *fmt) {
    return '<' == fmt[0] && ' ' == fmt[1];
}

void log_v2(int level, const char *file, int line, const char *func, const char *fmt, va_list args) {

    static const char *indent_str = "| | | | | | | | | | | | | | | | | | | | | | ";
    static int indent = 0;

    char str_buf[512];

    const char *fmt_skip_indent = fmt;

    if (is_add_indent(fmt)) {
        fmt_skip_indent += 2;
    } else if (is_sub_indent(fmt)) {
        indent -= 2;
        fmt_skip_indent += 2;
    }

    if (is_k_fn_name(func))
        snprintf(str_buf, sizeof(str_buf), "%s %.*s%s() %s  %s:%d\n"C_RESET, level_str(level), indent, indent_str, func, fmt_skip_indent, file, line);
    else
        snprintf(str_buf, sizeof(str_buf), "%s %.*s %s  %s:%d\n"C_RESET, level_str(level), indent, indent_str, fmt_skip_indent, file, line);

    if (is_add_indent(fmt)) {
        indent += 2;
    }

    vfprintf(stdout, str_buf, args);
}

/* endregion */

void k__log(int level, const char *file, int line, const char *func, const char *fmt, ...) {

    va_list args;
    va_start(args, fmt);
    log_v1(level, fmt, args);
    va_end(args);
}
