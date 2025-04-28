#ifndef K_STR_BUF_H
#define K_STR_BUF_H

#include <stddef.h>

#include "k_printf.h"

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

void k_str_buf_init(struct k_str_buf *buf, char *default_buf, size_t capacity);

void k_str_buf_free(struct k_str_buf *buf);

int k_str_buf_is_err(struct k_str_buf *buf);

char *k_str_buf_get(struct k_str_buf *buf);

int k_str_buf_puts(struct k_str_buf *buf, const char *str);

int k_str_buf_puts_n(struct k_str_buf *buf, const char *str, size_t len);

int k_str_buf_printf(struct k_str_buf *buf, const char *fmt, ...);

int k_str_buf_vprintf(struct k_str_buf *buf, const char *fmt, va_list args);

int k_str_buf_k_printf(struct k_str_buf *buf, k_printf_spec_match_fn fn_match, const char *fmt, ...);

int k_str_buf_k_vprintf(struct k_str_buf *buf, k_printf_spec_match_fn fn_match, const char *fmt, va_list args);

#endif
