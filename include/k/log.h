#ifndef K_LOG_H
#define K_LOG_H

#ifndef K__FILE_PATH
#define K__FILE_PATH __FILE__
#endif

void k__log(int level, const char *file, int line, const char *func, const char *fmt, ...);

#define k_log_trace(fmt, ...) k__log(0, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_debug(fmt, ...) k__log(1, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_info(fmt, ...)  k__log(2, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_warn(fmt, ...)  k__log(3, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_error(fmt, ...) k__log(4, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define k_log_fatal(fmt, ...) k__log(5, K__FILE_PATH, __LINE__, __func__, fmt, ##__VA_ARGS__)

#endif
