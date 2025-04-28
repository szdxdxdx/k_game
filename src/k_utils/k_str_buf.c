#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "k_str_buf.h"

static char buf_[1] = { '\0' };

void k_str_buf_init(struct k_str_buf *buf, char *default_buf, size_t capacity) {
    assert(NULL != buf);

    if (NULL != default_buf && 0 < capacity) {
        buf->default_buf = default_buf;
        buf->buf         = default_buf;
        buf->str_len     = 0;
        buf->capacity    = capacity;
    }
    else {
        buf->default_buf = buf_;
        buf->buf         = buf_;
        buf->str_len     = 0;
        buf->capacity    = 0;
    }
}

int k_str_buf_is_err(struct k_str_buf *buf) {
    assert(NULL != buf);
    return buf_ == buf->buf;
}

static void k__str_buf_set_err(struct k_str_buf *buf) {
    assert( ! k_str_buf_is_err(buf));

    if (buf->buf != buf->default_buf) {
        free(buf->buf);
    }

    buf->default_buf = NULL;
    buf->buf         = buf_;
    buf->str_len     = 0;
    buf->capacity    = 0;
}

void k_str_buf_free(struct k_str_buf *buf) {
    assert(NULL != buf);

    if (k_str_buf_is_err(buf))
        return;

    if (buf->buf != buf->default_buf) {
        free(buf->buf);
    }
}

char *k_str_buf_get(struct k_str_buf *buf) {
    assert(NULL != buf);
    return buf->buf;
}

size_t k_str_buf_get_len(struct k_str_buf *buf) {
    assert(NULL != buf);
    return buf->str_len;
}

static int k__str_buf_ensure_capacity(struct k_str_buf *buf, size_t add_len) {
    assert(NULL != buf->buf);

    size_t required_capacity = buf->str_len + add_len + 1;
    if (required_capacity < add_len)
        goto err;

    if (required_capacity <= buf->capacity)
        return 0;

    size_t new_capacity = buf->capacity * 2;

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

    memcpy(new_buf, buf->buf, buf->str_len);
    new_buf[buf->str_len] = '\0';

    if (buf->buf != buf->default_buf) {
        free(buf->buf);
    }

    buf->buf      = new_buf;
    buf->capacity = new_capacity;

    return 0;

err:
    k__str_buf_set_err(buf);
    return -1;
}

int k_str_buf_puts(struct k_str_buf *buf, const char *str) {
    return k_str_buf_puts_n(buf, str, strlen(str));
}

int k_str_buf_puts_n(struct k_str_buf *buf, const char *str, size_t len) {
    assert(NULL != buf);
    assert(NULL != str);

    if (k_str_buf_is_err(buf))
        return -1;

    if (0 != k__str_buf_ensure_capacity(buf, len))
        return -1;

    memcpy(&buf->buf[buf->str_len], str, len);
    buf->str_len += len;
    buf->buf[buf->str_len] = '\0';

    return 0;
}

int k_str_buf_printf(struct k_str_buf *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = k_str_buf_vprintf(buf, fmt, args);
    va_end(args);

    return r;
}

int k_str_buf_vprintf(struct k_str_buf *buf, const char *fmt, va_list args) {
    assert(NULL != buf);
    assert(NULL != fmt);

    if (k_str_buf_is_err(buf))
        return -1;

    size_t avail = buf->capacity - buf->str_len;

    va_list args_copy;
    va_copy(args_copy, args);
    int r = vsnprintf(&buf->buf[buf->str_len], avail, fmt, args);
    va_end(args_copy);

    if (r < 0) {
        k__str_buf_set_err(buf);
        return -1;
    }

    size_t add_len = r;

    if (add_len < avail) {
        buf->str_len += add_len;
        return 0;
    }

    if (0 != k__str_buf_ensure_capacity(buf, add_len))
        return -1;

    avail = buf->capacity - buf->str_len;

    r = vsnprintf(&buf->buf[buf->str_len], avail, fmt, args);

    if (r != add_len) {
        k__str_buf_set_err(buf);
        return -1;
    }

    buf->str_len += add_len;
    return 0;
}

int k_str_buf_k_printf(struct k_str_buf *buf, k_printf_spec_match_fn fn_match, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = k_str_buf_k_vprintf(buf, fn_match, fmt, args);
    va_end(args);

    return r;
}

int k_str_buf_k_vprintf(struct k_str_buf *buf, k_printf_spec_match_fn fn_match, const char *fmt, va_list args) {
    assert(NULL != buf);
    assert(NULL != fmt);

    if (k_str_buf_is_err(buf))
        return -1;

    size_t avail = buf->capacity - buf->str_len;

    va_list args_copy;
    va_copy(args_copy, args);
    int r = k_vsnprintf(fn_match, &buf->buf[buf->str_len], avail, fmt, args);
    va_end(args_copy);

    if (r < 0) {
        k__str_buf_set_err(buf);
        return -1;
    }

    size_t add_len = r;

    if (add_len < avail) {
        buf->str_len += add_len;
        return 0;
    }

    if (0 != k__str_buf_ensure_capacity(buf, add_len))
        return -1;

    avail = buf->capacity - buf->str_len;

    r = k_vsnprintf(fn_match, &buf->buf[buf->str_len], avail, fmt, args);

    if (r != add_len) {
        k__str_buf_set_err(buf);
        return -1;
    }

    buf->str_len += add_len;
    return 0;
}
