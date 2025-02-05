#include <stdarg.h>
#include <stdio.h>

#include "k/log.h"

#define C_RESET   "\033[0m"
#define C_RED     "\033[91m"
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_BLUE    "\033[94m"
#define C_MAGENTA "\033[95m"
#define C_CYAN    "\033[36m"
#define C_WHITE   "\033[37m"

static const char *level_s[] = {
    C_MAGENTA "T",
    C_MAGENTA "D",
    C_CYAN    "I",
    C_YELLOW  "W",
    C_RED     "E",
    C_RED     "F",
};

static inline int is_k_fn_name(const char *fn) {
    return fn[0] == 'k' && fn[1] == '_' && fn[2] != '_';
}

void k__log(int level, const char *file, int line, const char *func, const char *fmt, ...) {

    char fmt_buf[512];
    if (is_k_fn_name(func)) {
        snprintf(fmt_buf, sizeof(fmt_buf), "%s %s() " C_RESET "%s | %s:%d\n", level_s[level], func, fmt, file, line);
    }
    else {
        snprintf(fmt_buf, sizeof(fmt_buf), "%s " C_RESET "%s | %s:%d\n", level_s[level], fmt, file, line);
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt_buf, args);
    va_end(args);
}
