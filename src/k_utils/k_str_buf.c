#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "k_str_buf.h"

struct k_str_buf *k_str_buf_init(struct k_str_buf *str_buf, char *default_buf, size_t capacity) {
    assert(NULL != str_buf);

    if (NULL != default_buf && 0 < capacity) {
        str_buf->default_buf = default_buf;
        str_buf->buf         = default_buf;
        str_buf->str_len     = 0;
        str_buf->capacity    = capacity;
    }
    else {
        static char buf_[1] = { '\0' };
        str_buf->default_buf = buf_;
        str_buf->buf         = buf_;
        str_buf->str_len     = 0;
        str_buf->capacity    = 0;
    }
}

static inline int k__str_buf_is_err(struct k_str_buf *str_buf) {
    return NULL == str_buf->buf;
}

static void k__str_buf_set_err(struct k_str_buf *str_buf) {
    assert( ! k__str_buf_is_err(str_buf));

    if (str_buf->buf != str_buf->default_buf) {
        free(str_buf->buf);
    }

    str_buf->default_buf = NULL;
    str_buf->buf         = NULL;
    str_buf->str_len     = 0;
    str_buf->capacity    = 0;
}

void k_str_buf_free(struct k_str_buf *str_buf) {
    assert(NULL != str_buf);

    if (k__str_buf_is_err(str_buf))
        return;

    if (str_buf->buf != str_buf->default_buf) {
        free(str_buf->buf);
    }
}

char *k_str_buf_get(struct k_str_buf *str_buf) {
    assert(NULL != str_buf);
    return str_buf->buf;
}

static int k__str_buf_ensure_capacity(struct k_str_buf *str_buf, size_t add_len) {
    assert(NULL != str_buf->buf);

    size_t required_capacity = str_buf->str_len + add_len + 1;
    if (required_capacity < add_len)
        goto err;

    if (required_capacity <= str_buf->capacity)
        return 0;

    size_t new_capacity = str_buf->capacity * 2;

    if (new_capacity < required_capacity) {
        new_capacity = required_capacity;
    }

    char *new_buf = malloc(new_capacity);
    if (NULL == new_buf) {
        if (new_capacity == required_capacity) {
            goto err;
        }
        else {
            new_capacity = required_capacity;
            new_buf = malloc(new_capacity);
            if (NULL == new_buf)
                goto err;
        }
    }

    memcpy(new_buf, str_buf->buf, str_buf->str_len);
    new_buf[str_buf->str_len] = '\0';

    if (str_buf->buf != str_buf->default_buf) {
        free(str_buf->buf);
    }

    str_buf->buf      = new_buf;
    str_buf->capacity = new_capacity;

    return 0;

err:
    k__str_buf_set_err(str_buf);
    return -1;
}

int k_str_buf_puts(struct k_str_buf *str_buf, const char *str) {
    return k_str_buf_puts_n(str_buf, str, strlen(str));
}

int k_str_buf_puts_n(struct k_str_buf *str_buf, const char *str, size_t len) {
    assert(NULL != str_buf);
    assert(NULL != str);

    if (k__str_buf_is_err(str_buf))
        return -1;

    if (0 != k__str_buf_ensure_capacity(str_buf, len))
        return -1;

    memcpy(&str_buf->buf[str_buf->str_len], str, len);
    str_buf->str_len += len;
    str_buf->buf[str_buf->str_len] = '\0';

    return 0;
}

int k_str_buf_printf(struct k_str_buf *str_buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = k_str_buf_vprintf(str_buf, fmt, args);
    va_end(args);

    return r;
}

int k_str_buf_vprintf(struct k_str_buf *str_buf, const char *fmt, va_list args) {
    assert(NULL != str_buf);
    assert(NULL != fmt);

    if (k__str_buf_is_err(str_buf))
        return -1;

    size_t avail = str_buf->capacity - str_buf->str_len;

    va_list args_copy;
    va_copy(args_copy, args);
    int r = vsnprintf(&str_buf->buf[str_buf->str_len], avail, fmt, args);
    va_end(args_copy);

    if (r < 0) {
        k__str_buf_set_err(str_buf);
        return -1;
    }

    size_t add_len = r;

    if (add_len < avail) {
        str_buf->str_len += add_len;
        return 0;
    }

    if (0 != k__str_buf_ensure_capacity(str_buf, add_len))
        return -1;

    avail = str_buf->capacity - str_buf->str_len;

    r = vsnprintf(&str_buf->buf[str_buf->str_len], avail, fmt, args);

    if (r != add_len) {
        k__str_buf_set_err(str_buf);
        return -1;
    }

    str_buf->str_len += add_len;
    return 0;
}
