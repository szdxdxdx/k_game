#include <stdarg.h>
#include <stdio.h>

#include "k_log.h"

#if K_LOG_COLOR
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
        case K_LOG_LEVEL_TRACE: return C_MAGENTA "TRACE" C_RESET;
        case K_LOG_LEVEL_DEBUG: return C_MAGENTA "DEBUG" C_RESET;
        case K_LOG_LEVEL_INFO : return C_CYAN    "INFO " C_RESET;
        case K_LOG_LEVEL_WARN : return C_YELLOW  "WARN " C_RESET;
        case K_LOG_LEVEL_ERROR: return C_RED     "ERROR" C_RESET;
        case K_LOG_LEVEL_FATAL: return C_RED     "FATAL" C_RESET;
        default:                return C_CYAN    "INFO " C_RESET;
    }
}

static void log_v1(int level, const char *fmt, va_list args) {

    char buf[512];
    snprintf(buf, sizeof(buf), "%s %s\n", level_str(level), fmt);
    vfprintf(stdout, buf, args);
}

static void log_v2(int level, const char *file, int line, const char *fn, const char *fmt, va_list args) {

    char buf[512];
    snprintf(buf, sizeof(buf), "%s %s() %s:%d\n%s\n", level_str(level), fn, file, line, fmt);
    vfprintf(stdout, buf, args);
}

static void log_v3(int level, const char *fn, const char *file, int line, const char *fmt, va_list args) {

    char buf[800];
    snprintf(buf, sizeof(buf), "%s " C_MAGENTA " %s() %s:%d\n" C_RESET "%s\n" , level_str(level), fn, file, line, fmt);
    vfprintf(stdout, buf, args);
}

void k__log(int level, const char *file, int line, const char *fn, const char *fmt, ...) {

    va_list args;
    va_start(args, fmt);
    // log_v1(level, fmt, args);
    // log_v2(level, file, line, fn, fmt, args);
    log_v3(level, fn, file, line, fmt, args);
    va_end(args);
}
