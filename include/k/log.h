#ifndef K_LOG_H
#define K_LOG_H

#ifndef K__FILE_PATH
#define K__FILE_PATH __FILE__
#endif

#define K_LOG_LEVEL_TRACE 0
#define K_LOG_LEVEL_DEBUG 1
#define K_LOG_LEVEL_INFO  2
#define K_LOG_LEVEL_WARN  3
#define K_LOG_LEVEL_ERROR 4
#define K_LOG_LEVEL_FATAL 5
#define K_LOG_LEVEL_OFF   6

#ifndef K_LOG_LEVEL
#define K_LOG_LEVEL K_LOG_LEVEL_TRACE
#endif

void k__log(int level, const char *file, int line, const char *func, const char *fmt, ...);

#define k_log_trace(fmt, ...) k__log(K_LOG_LEVEL_TRACE, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_debug(fmt, ...) k__log(K_LOG_LEVEL_DEBUG, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_info(fmt, ...)  k__log(K_LOG_LEVEL_INFO , K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_warn(fmt, ...)  k__log(K_LOG_LEVEL_WARN , K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_error(fmt, ...) k__log(K_LOG_LEVEL_ERROR, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_fatal(fmt, ...) k__log(K_LOG_LEVEL_FATAL, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)

#if K_LOG_LEVEL > K_LOG_LEVEL_TRACE
#undef k_log_trace
#define k_log_trace(fmt, ...)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_DEBUG
#undef k_log_debug
#define k_log_debug(fmt, ...)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_INFO
#undef k_log_info
#define k_log_info(fmt, ...)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_WARN
#undef k_log_warn
#define k_log_warn(fmt, ...)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_ERROR
#undef k_log_error
#define k_log_error(fmt, ...)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_FATAL
#undef k_log_fatal
#define k_log_fatal(fmt, ...)
#endif

#endif
