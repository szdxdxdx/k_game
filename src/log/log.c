#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "k/log.h"

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
#define C_RESET   ""
#define C_RED     ""
#define C_GREEN   ""
#define C_YELLOW  ""
#define C_BLUE    ""
#define C_MAGENTA ""
#define C_CYAN    ""
#define C_WHITE   ""
#endif

#define LEVEL_STR_TRACE C_MAGENTA "T"
#define LEVEL_STR_DEBUG C_MAGENTA "D"
#define LEVEL_STR_INFO  C_CYAN    "I"
#define LEVEL_STR_WARN  C_YELLOW  "W"
#define LEVEL_STR_ERROR C_RED     "E"
#define LEVEL_STR_FATAL C_RED     "F"

static inline const char *level_str(int level) {

    static const char *level_s[] = {
        LEVEL_STR_TRACE,
        LEVEL_STR_DEBUG,
        LEVEL_STR_INFO ,
        LEVEL_STR_WARN ,
        LEVEL_STR_ERROR,
        LEVEL_STR_FATAL,
    };

    switch(level) {
        case K_LOG_LEVEL_TRACE: return LEVEL_STR_TRACE;
        case K_LOG_LEVEL_DEBUG: return LEVEL_STR_DEBUG;
        case K_LOG_LEVEL_INFO : return LEVEL_STR_INFO;
        case K_LOG_LEVEL_WARN : return LEVEL_STR_WARN;
        case K_LOG_LEVEL_ERROR: return LEVEL_STR_ERROR;
        case K_LOG_LEVEL_FATAL: return LEVEL_STR_FATAL;
        default:                return LEVEL_STR_INFO;
    }
}

static inline int is_k_fn_name(const char *fn) {
    return fn[0] == 'k' && fn[1] == '_' && fn[2] != '_';
}

static inline int is_add_indent(const char *fmt) {
    return '>' == fmt[0] && ' ' == fmt[1];
}

static inline int is_sub_indent(const char *fmt) {
    return '<' == fmt[0] && ' ' == fmt[1];
}

static const char *indent_str = "| | | | | | | | | | | | | | | | | | | | | | ";

static int indent = 0;

void k__log(int level, const char *file, int line, const char *func, const char *fmt, ...) {

    char str_buf[512];

    const char *fmt_skip_indent = fmt;

    if (is_add_indent(fmt)) {
        fmt_skip_indent += 2;
    } else if (is_sub_indent(fmt)) {
        indent -= 2;
        fmt_skip_indent += 2;
    }

    if (is_k_fn_name(func))
        snprintf(str_buf, sizeof(str_buf), "%s %.*s%s() "C_RESET"%s    %s:%d\n", level_str(level), indent, indent_str, func, fmt_skip_indent, file, line);
    else
        snprintf(str_buf, sizeof(str_buf), "%s %.*s"C_RESET"%s    %s:%d\n", level_str(level), indent, indent_str, fmt_skip_indent, file, line);

    if (is_add_indent(fmt)) {
        indent += 2;
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, str_buf, args);
    va_end(args);
}
