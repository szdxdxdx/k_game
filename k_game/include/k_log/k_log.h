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

#define K_LOG_LEVEL_ALL   0
#define K_LOG_LEVEL_OFF   6

/* 定义日志的级别，低于此级别的日志输出不参与编译 */
#ifndef K_LOG_LEVEL
#define K_LOG_LEVEL K_LOG_LEVEL_ALL
#endif

/* 是否彩色打印日志，1 为彩色打印，0 为 */
#ifndef K_LOG_COLOR
#define K_LOG_COLOR 1
#endif

/* 日志的标签前缀 */
#ifndef K_LOG_TAG
#define K__LOG_TAG ""
#else
#define K__LOG_TAG "[" K_LOG_TAG "] "
#endif

void k__log(int level, const char *file, int line, const char *fn, const char *fmt, ...);

/**
 * \brief 格式化打印日志
 *
 * `fmt` 为格式字符串，用法类似于 `printf()`，示例：
 * ```C
 * k_log_info("1+1=%d", 2);
 * ```
 */
#define k_log_trace(fmt, ...) k__log(K_LOG_LEVEL_TRACE, K__FILE_PATH, __LINE__, __func__, K__LOG_TAG fmt, ##__VA_ARGS__)
#define k_log_debug(fmt, ...) k__log(K_LOG_LEVEL_DEBUG, K__FILE_PATH, __LINE__, __func__, K__LOG_TAG fmt, ##__VA_ARGS__)
#define k_log_info(fmt, ...)  k__log(K_LOG_LEVEL_INFO , K__FILE_PATH, __LINE__, __func__, K__LOG_TAG fmt, ##__VA_ARGS__)
#define k_log_warn(fmt, ...)  k__log(K_LOG_LEVEL_WARN , K__FILE_PATH, __LINE__, __func__, K__LOG_TAG fmt, ##__VA_ARGS__)
#define k_log_error(fmt, ...) k__log(K_LOG_LEVEL_ERROR, K__FILE_PATH, __LINE__, __func__, K__LOG_TAG fmt, ##__VA_ARGS__)
#define k_log_fatal(fmt, ...) k__log(K_LOG_LEVEL_FATAL, K__FILE_PATH, __LINE__, __func__, K__LOG_TAG fmt, ##__VA_ARGS__)

#if K_LOG_LEVEL > K_LOG_LEVEL_TRACE
#undef  k_log_trace
#define k_log_trace(fmt, ...) ((void)0)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_DEBUG
#undef  k_log_debug
#define k_log_debug(fmt, ...) ((void)0)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_INFO
#undef  k_log_info
#define k_log_info(fmt, ...) ((void)0)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_WARN
#undef  k_log_warn
#define k_log_warn(fmt, ...) ((void)0)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_ERROR
#undef  k_log_error
#define k_log_error(fmt, ...) ((void)0)
#undef  k_log_error_once
#define k_log_error_once(fmt, ...) ((void)0)
#endif

#if K_LOG_LEVEL > K_LOG_LEVEL_FATAL
#undef  k_log_fatal
#define k_log_fatal(fmt, ...) ((void)0)
#endif

#endif
