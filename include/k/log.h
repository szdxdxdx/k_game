#ifndef K_LOG_H
#define K_LOG_H

#include <stdio.h>

#ifndef K__FILE_PATH
#define K__FILE_PATH __FILE__
#endif

#define K_LOG_COLOR_RESET   "\033[0m"
#define K_LOG_COLOR_RED     "\033[91m"
#define K_LOG_COLOR_GREEN   "\033[32m"
#define K_LOG_COLOR_YELLOW  "\033[33m"
#define K_LOG_COLOR_BLUE    "\033[94m"
#define K_LOG_COLOR_MAGENTA "\033[95m"
#define K_LOG_COLOR_CYAN    "\033[36m"
#define K_LOG_COLOR_WHITE   "\033[37m"

#define k_log_trace(fmt, ...) fprintf(stdout, K_LOG_COLOR_MAGENTA "T" K_LOG_COLOR_RESET " " fmt " | %s:%d\n", ##__VA_ARGS__, K__FILE_PATH, __LINE__)
#define k_log_debug(fmt, ...) fprintf(stdout, K_LOG_COLOR_MAGENTA "D" K_LOG_COLOR_RESET " " fmt " | %s:%d\n", ##__VA_ARGS__, K__FILE_PATH, __LINE__)
#define k_log_info(fmt, ...)  fprintf(stdout, K_LOG_COLOR_CYAN    "I" K_LOG_COLOR_RESET " " fmt " | %s:%d\n", ##__VA_ARGS__, K__FILE_PATH, __LINE__)
#define k_log_warn(fmt, ...)  fprintf(stdout, K_LOG_COLOR_YELLOW  "W" K_LOG_COLOR_RESET " " fmt " | %s:%d\n", ##__VA_ARGS__, K__FILE_PATH, __LINE__)
#define k_log_error(fmt, ...) fprintf(stdout, K_LOG_COLOR_RED     "E" K_LOG_COLOR_RESET " " fmt " | %s:%d\n", ##__VA_ARGS__, K__FILE_PATH, __LINE__)
#define k_log_fatal(fmt, ...) fprintf(stdout, K_LOG_COLOR_RED     "F" K_LOG_COLOR_RESET " " fmt " | %s:%d\n", ##__VA_ARGS__, K__FILE_PATH, __LINE__)

#endif
