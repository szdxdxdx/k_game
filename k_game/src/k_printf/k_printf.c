#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "k_printf.h"

/* region [printf_buf] */

struct k_printf_buf {

    void (*fn_puts_n)(struct k_printf_buf *buf, const char *str, size_t len);

    void (*fn_vprintf)(struct k_printf_buf *buf, const char *fmt, va_list args);

    /* 截至目前应打印出的字符的数量（忽略缓冲区的实际大小） */
    int n;
};

/* region [str_buf] */

struct k_printf_str_buf {

    struct k_printf_buf printf_buf;

    char *buf;

    /* 当前缓冲区中字符串的长度 */
    int str_len;

    /* 能容纳的最长字符串的长度，等于缓冲区容量减 1，因为要容纳结尾的 `\0` */
    int max_len;
};

static void str_buf_puts_n(struct k_printf_buf *printf_buf, const char *str, size_t len) {

    if (-1 == printf_buf->n)
        return;

    struct k_printf_str_buf *str_buf = (struct k_printf_str_buf *)printf_buf;

    int available = str_buf->max_len - str_buf->str_len;

    if (len <= available) {
        memcpy(&str_buf->buf[str_buf->str_len], str, len);
        str_buf->str_len += (int)len;
        str_buf->buf[str_buf->str_len] = '\0';

        printf_buf->n += (int)len;
        return;
    }

    if (0 < available) {
        memcpy(&str_buf->buf[str_buf->str_len], str, available);
        str_buf->str_len = str_buf->max_len;
    }

    str_buf->buf[str_buf->str_len] = '\0';

    if (len <= INT_MAX) {
        printf_buf->n += (int)len;
        if (printf_buf->n < 0)
            printf_buf->n = -1;
    } else {
        printf_buf->n = -1;
    }
}

static void str_buf_vprintf(struct k_printf_buf *printf_buf, const char *fmt, va_list args) {

    if (-1 == printf_buf->n)
        return;

    struct k_printf_str_buf *str_buf = (struct k_printf_str_buf *)printf_buf;

    int available = str_buf->max_len - str_buf->str_len;
    int r = vsnprintf(&str_buf->buf[str_buf->str_len], available + 1, fmt, args);
    if (r < 0) {
        printf_buf->n = -1;
        return;
    }

    if (r <= available) {
        str_buf->str_len += r;
        printf_buf->n += r;
    } else {
        str_buf->str_len = str_buf->max_len;
        printf_buf->n += r;
        if (printf_buf->n < 0)
            printf_buf->n = -1;
    }
}

static void str_buf_init(struct k_printf_str_buf *str_buf, char *buf, size_t capacity) {

    str_buf->printf_buf.fn_puts_n  = str_buf_puts_n,
    str_buf->printf_buf.fn_vprintf = str_buf_vprintf,
    str_buf->printf_buf.n          = 0;

    if (1 < capacity && capacity <= INT_MAX) {
        str_buf->buf     = buf;
        str_buf->str_len = 0;
        str_buf->max_len = (int)capacity - 1;
    } else {
        static char buf_[1] = { '\0' };
        str_buf->buf     = buf_;
        str_buf->str_len = 0;
        str_buf->max_len = 0;
    }

    str_buf->buf[0] = '\0';
}

/* endregion */

/* region [file_buf] */

struct k_printf_file_buf {

    struct k_printf_buf printf_buf;

    FILE *file;
};

static void file_buf_puts_n(struct k_printf_buf *printf_buf, const char *str, size_t len) {

    if (-1 == printf_buf->n)
        return;

    struct k_printf_file_buf *file_buf = (struct k_printf_file_buf *)printf_buf;

    size_t r = fwrite(str, sizeof(char), len, file_buf->file);
    if (INT_MAX < r) {
        printf_buf->n = -1;
        return;
    }

    printf_buf->n += (int)r;
    if (printf_buf->n < 0)
        printf_buf->n = -1;
}

static void file_buf_vprintf(struct k_printf_buf *printf_buf, const char *fmt, va_list args) {

    if (-1 == printf_buf->n)
        return;

    struct k_printf_file_buf *file_buf = (struct k_printf_file_buf *)printf_buf;

    int r = vfprintf(file_buf->file, fmt, args);
    if (r < 0) {
        printf_buf->n = -1;
        return;
    }

    printf_buf->n += r;
    if (printf_buf->n < 0)
        printf_buf->n = -1;
}

static void file_buf_init(struct k_printf_file_buf *printf_buf, FILE *file) {

    printf_buf->printf_buf.fn_puts_n  = file_buf_puts_n,
    printf_buf->printf_buf.fn_vprintf = file_buf_vprintf,
    printf_buf->printf_buf.n          = 0;
    printf_buf->file                  = file;
}

/* endregion */

void k_printf_buf_puts(struct k_printf_buf *buf, const char *str) {
    assert(NULL != buf);
    assert(NULL != str);

    buf->fn_puts_n(buf, str, strlen(str));
}

void k_printf_buf_puts_n(struct k_printf_buf *buf, const char *str, size_t len) {
    assert(NULL != buf);
    assert(NULL != str);

    buf->fn_puts_n(buf, str, len);
}

void k_printf_buf_printf(struct k_printf_buf *buf, const char *fmt, ...) {
    assert(NULL != buf);
    assert(NULL != fmt);

    va_list args;
    va_start(args, fmt);
    buf->fn_vprintf(buf, fmt, args);
    va_end(args);
}

void k_printf_buf_vprintf(struct k_printf_buf *buf, const char *fmt, va_list args) {
    assert(NULL != buf);
    assert(NULL != fmt);

    buf->fn_vprintf(buf, fmt, args);
}

int k_printf_buf_get_n(struct k_printf_buf *buf) {
    assert(NULL != buf);
    return buf->n;
}

/* endregion */

/* region [c_std_spec] */

/* 处理 C printf 中 `%n` 一族的格式说明符 */
static void k__printf_c_spec_get_n(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args) {

    const char c1 = spec->type[0];
    const char c2 = spec->type[1];

    int n = buf->n;

    if (c1=='n') {
        *(va_arg(*args, int *)) = (int)n; /* %n */
    } else if (c1=='j') {
        *(va_arg(*args, intmax_t *)) = (intmax_t)n; /* %jn */
    } else if (c1=='t') {
        *(va_arg(*args, ptrdiff_t *)) = (ptrdiff_t)n; /* %tn */
    } else if (c1=='z') {
        *(va_arg(*args, size_t *)) = (size_t)n; /* %zn */
    } else if (c1=='l') {
        if (c2=='l') {
            *(va_arg(*args, long long *)) = (long long)n; /* %lln */
        } else {
            *(va_arg(*args, long *)) = (long)n; /* %ln */
        }
    } else {
        if (c2 == 'h') {
            *(va_arg(*args, unsigned char *)) = (unsigned char)n; /* %hhn */
        } else {
            *(va_arg(*args, short *)) = (short)n; /* %hn */
        }
    }
}

/* 处理 C printf 中除了 `%n` 一族以外所有的格式说明符 */
static void k__printf_c_spec_print(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args) {

    char fmt_buf[96];
    char *fmt = fmt_buf;

    int len = (int)(spec->end - spec->start);
    if (sizeof(fmt_buf) < len + 1) {
        if (NULL == (fmt = malloc(len + 1))) {
            buf->n = -1;
            return;
        }
    }

    memcpy(fmt, spec->start, len);
    fmt[len] = '\0';

    va_list args_copy;
    va_copy(args_copy, *args);
    buf->fn_vprintf(buf, fmt, args_copy);
    va_end(args_copy);

    if (fmt != fmt_buf)
        free(fmt);

    if (spec->use_min_width && -1 == spec->min_width)
        va_arg(*args, int);
    if (spec->use_precision && -1 == spec->precision)
        va_arg(*args, int);

    switch(spec->type[0]) {
        case 'a': case 'A': case 'e': case 'E':
        case 'f': case 'F': case 'g': case 'G':
            va_arg(*args, double);
            break;
        case 'c': case 'd': case 'i': case 'h':
            va_arg(*args, int);
            break;
        case 'o': case 'u': case 'x': case 'X':
            va_arg(*args, unsigned int);
            break;
        case 'j':
            switch (spec->type[1]) {
                case 'd': case 'i':
                    va_arg(*args, intmax_t);
                    break;
                case 'x': case 'X':
                case 'o': case 'u':
                    va_arg(*args, uintmax_t);
                    break;
            }
            break;
        case 'l':
            switch (spec->type[1]) {
                case 'a': case 'A': case 'e': case 'E':
                case 'f': case 'F': case 'g': case 'G':
                    va_arg(*args, double);
                    break;
                case 'c':
                    va_arg(*args, int);
                    break;
                case 'd': case 'i':
                    va_arg(*args, long);
                    break;
                case 'o': case 'u': case 'x': case 'X':
                    va_arg(*args, unsigned long);
                    break;
                case 's':
                    va_arg(*args, wchar_t *);
                    break;
                case 'l':
                    switch (spec->type[2]) {
                        case 'd': case 'i':
                            va_arg(*args, long long);
                            break;
                        case 'o': case 'u':
                        case 'x': case 'X':
                            va_arg(*args, unsigned long long);
                            break;
                    }
                    break;
            }
            break;
        case 'L':
            va_arg(*args, long double);
            break;
        case 'p':
            va_arg(*args, void *);
            break;
        case 's':
            va_arg(*args, char *);
            break;
        case 't':
            va_arg(*args, ptrdiff_t);
            break;
        case 'z':
            va_arg(*args, size_t);
            break;
    }
}

/* 匹配 C printf 格式说明符，若匹配成功则移动字符串指针，并返回对应的回调 */
static k_printf_spec_print_fn match_c_std_spec(const char **str) {

    switch ((*str)[0]) {
        case 'a': case 'A': case 'c': case 'd':
        case 'e': case 'E': case 'f': case 'F':
        case 'g': case 'G': case 'i': case 'o':
        case 'p': case 's': case 'u': case 'x':
        case 'X':
            *str += 1;
            return k__printf_c_spec_print;

        case 'n':
            *str += 1;
            return k__printf_c_spec_get_n;

        case 'h': {
            switch ((*str)[1]) {
                case 'd': case 'i':
                case 'o': case 'u':
                case 'x': case 'X':
                    *str += 2;
                    return k__printf_c_spec_print;
                case 'n':
                    *str += 2;
                    return k__printf_c_spec_get_n;
                case 'h': {
                    switch ((*str)[2]) {
                        case 'd': case 'i':
                        case 'o': case 'u':
                        case 'x': case 'X':
                            *str += 3;
                            return k__printf_c_spec_print;
                        case 'n':
                            *str += 3;
                            return k__printf_c_spec_get_n;
                    }
                    break;
                }
            }
            break;
        }

        case 'l': {
            switch ((*str)[1]) {
                case 'a': case 'A': case 'c': case 'd':
                case 'e': case 'E': case 'f': case 'F':
                case 'g': case 'G': case 'i': case 'o':
                case 's': case 'u': case 'x': case 'X':
                    *str += 2;
                    return k__printf_c_spec_print;
                case 'n':
                    *str += 2;
                    return k__printf_c_spec_get_n;
                case 'l': {
                    switch ((*str)[2]) {
                        case 'd': case 'i':
                        case 'o': case 'u':
                        case 'x': case 'X':
                            *str += 3;
                            return k__printf_c_spec_print;
                        case 'n':
                            *str += 3;
                            return k__printf_c_spec_get_n;
                    }
                    break;
                }
            }
            break;
        }

        case 'L': {
            switch ((*str)[1]) {
                case 'a': case 'A': case 'e': case 'E':
                case 'f': case 'F': case 'g': case 'G':
                    *str += 2;
                    return k__printf_c_spec_print;
            }
            break;
        }

        case 'j': case 't': case 'z': {
            switch ((*str)[1]) {
                case 'd': case 'i':
                case 'o': case 'u':
                case 'x': case 'X':
                    *str += 2;
                    return k__printf_c_spec_print;
                case 'n':
                    *str += 2;
                    return k__printf_c_spec_get_n;
            }
            break;
        }
    }

    return NULL;
}

/* endregion */

/* region [user_spec] */

k_printf_spec_print_fn k_printf_spec_match_helper(const struct k_printf_spec_print_pair *pairs, const char **str) {
    assert(NULL != pairs);
    assert(NULL != str);

    const struct k_printf_spec_print_pair *pair = pairs;
    while (NULL != pair->spec_type) {

        const char *p1 = *str;
        const char *p2 = pair->spec_type;
        for (; '\0' != *p2; p1++, p2++) {

            if (*p1 != *p2)
                goto next_spec;
        }

        *str += p2 - pair->spec_type;

        assert(NULL != pair->fn_print);
        return pair->fn_print;

    next_spec:
        pair++;
    }

    assert(NULL == pair->fn_print);
    return NULL;
}

/* endregion */

/* region [k__printf] */

static int extract_int(const char **str) {

    unsigned long long num = 0;

    const char *ch = *str;
    assert('1' <= *ch && *ch <= '9');

    for (; '0' <= *ch && *ch <= '9'; ch++) {
        num *= 10;
        num += *ch - '0';

        if (INT_MAX <= num) {

            while ('0' <= *ch && *ch <= '9') {
                ch++;
            }

            num = INT_MAX;
            break;
        }
    }

    assert(0 <= num && num <= INT_MAX);

    *str = ch;
    return (int)num;
}

static k_printf_spec_print_fn extract_spec(k_printf_spec_match_fn fn_match, const char **str, struct k_printf_spec *get_spec) {
    assert('%' == **str);

    const char *ch = *str + 1;

    struct k_printf_spec spec;
    spec.start = *str;

    spec.left_justified   = 0;
    spec.sign_prepended   = 0;
    spec.space_padded     = 0;
    spec.zero_padding     = 0;
    spec.alternative_form = 0;
    for (;;) {
        switch (*ch) {
            case '-': ch++; spec.left_justified   = 1; continue;
            case '+': ch++; spec.sign_prepended   = 1; continue;
            case ' ': ch++; spec.space_padded     = 1; continue;
            case '0': ch++; spec.zero_padding     = 1; continue;
            case '#': ch++; spec.alternative_form = 1; continue;
        }
        break;
    }

    if ('1' <= *ch && *ch <= '9') {
        spec.use_min_width = 1;
        spec.min_width     = extract_int(&ch);
    } else if ('*' == *ch) {
        ch++;
        spec.use_min_width = 1;
        spec.min_width     = -1;
    } else {
        spec.use_min_width = 0;
        spec.min_width     = -1;
    }

    if ('.' == *ch) {
        ch++;
        if ('0' <= *ch && *ch <= '9') {
            spec.use_precision = 1;
            spec.precision     = extract_int(&ch);
        } else if ('*' == *ch) {
            ch++;
            spec.use_precision = 1;
            spec.precision     = -1;
        } else {
            return NULL;
        }
    } else {
        spec.use_precision = 0;
        spec.precision     = -1;
    }

    spec.type = ch;

    k_printf_spec_print_fn fn_callback = fn_match(&ch);
    if (NULL == fn_callback) {

        fn_callback = match_c_std_spec(&ch);
        if (NULL == fn_callback)
            return NULL;
    }

    spec.end = ch;

    *str = ch;
    *get_spec = spec;
    return fn_callback;
}

/* 格式化写入字符串到缓冲区，并返回格式化后的字符串长度
 *
 * 本函数是 k_printf 家族所有函数的核心实现。
 */
static int k__printf(k_printf_spec_match_fn fn_match, struct k_printf_buf *buf, const char *fmt, va_list args) {

    const char *s = fmt;
    const char *p = s;
    for (;;) {
        while ('%' != *p && '\0' != *p) {
            ++p;
        }

        if (s < p) {
            buf->fn_puts_n(buf, s, p - s);
        }

        if ('\0' == *p)
            break;

        if ('%' == *(p + 1)) {
            s = p + 1;
            p = p + 2;
            continue;
        }

        s = p;

        struct k_printf_spec spec;
        k_printf_spec_print_fn fn_callback = extract_spec(fn_match, &s, &spec);
        if (NULL != fn_callback) {
            fn_callback(buf, &spec, &args);
            p = s;
        } else {
            p = s + 1;
        }
    }

    return buf->n;
}

/* endregion */

/* region [k_printf] */

int k_printf(k_printf_spec_match_fn fn_match, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = k_vfprintf(fn_match, stdout, fmt, args);
    va_end(args);

    return r;
}

int k_fprintf(k_printf_spec_match_fn fn_match, FILE *file, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = k_vfprintf(fn_match, file, fmt, args);
    va_end(args);

    return r;
}

int k_vfprintf(k_printf_spec_match_fn fn_match, FILE *file, const char *fmt, va_list args) {
    assert(NULL != file);
    assert(NULL != fmt);

    if (NULL == fn_match)
        return vfprintf(file, fmt, args);

    struct k_printf_file_buf file_buf;
    file_buf_init(&file_buf, file);

    return k__printf(fn_match, &file_buf.printf_buf, fmt, args);
}

int k_sprintf(k_printf_spec_match_fn fn_match, char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = k_vsprintf(fn_match, buf, fmt, args);
    va_end(args);

    return r;
}

int k_vsprintf(k_printf_spec_match_fn fn_match, char *buf, const char *fmt, va_list args) {
    return k_vsnprintf(fn_match, buf, INT_MAX, fmt, args);
}

int k_snprintf(k_printf_spec_match_fn fn_match, char *buf, size_t n, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = k_vsnprintf(fn_match, buf, n, fmt, args);
    va_end(args);

    return r;
}

int k_vsnprintf(k_printf_spec_match_fn fn_match, char *buf, size_t n, const char *fmt, va_list args) {
    assert(NULL != fmt);

    if (NULL == fn_match)
        return vsnprintf(buf, n, fmt, args);

    struct k_printf_str_buf str_buf;
    str_buf_init(&str_buf, buf, n);

    return k__printf(fn_match, &str_buf.printf_buf, fmt, args);
}

int k_asprintf(k_printf_spec_match_fn fn_match, char **get_s, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = k_vasprintf(fn_match, get_s, fmt, args);
    va_end(args);

    return r;
}

int k_vasprintf(k_printf_spec_match_fn fn_match, char **get_s, const char *fmt, va_list args) {
    assert(NULL != get_s);
    assert(NULL != fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    int str_len = k_vsnprintf(fn_match, NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (str_len <= 0 || str_len == INT_MAX)
        return -1;

    char *buf = malloc(str_len + 1);
    if (NULL == buf)
        return -1;

    if (str_len != k_vsnprintf(fn_match, buf, str_len + 1, fmt, args)) {
        free(buf);
        return -1;
    }

    *get_s = buf;
    return str_len;
}

/* endregion */
