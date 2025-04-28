#ifndef K_STR_BUF_H
#define K_STR_BUF_H

#include <stddef.h>

struct k_str_buf {

    /** \brief [private] */
    char *default_buf;

    /** \brief [private] */
    char *alloc_buf;

    /** \brief [read-only] */
    char *buf;

    /** \brief [read-only] */
    size_t str_len;

    /** \brief [private] */
    size_t capacity;
};

struct k_str_buf *k_str_buf_init(struct k_str_buf *str_buf, char *default_buf, size_t capacity);

void k_str_buf_free(struct k_str_buf *str_buf);

char *k_str_buf_get(struct k_str_buf *str_buf);

int k_str_buf_puts(struct k_str_buf *str_buf, const char *str);

int k_str_buf_puts_n(struct k_str_buf *str_buf, const char *str, size_t len);

int k_str_buf_printf(struct k_str_buf *str_buf, const char *fmt, ...);

int k_str_buf_vprintf(struct k_str_buf *str_buf, const char *fmt, va_list args);

#endif
