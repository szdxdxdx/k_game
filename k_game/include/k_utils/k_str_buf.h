#ifndef K_STR_BUF_H
#define K_STR_BUF_H

#include <stddef.h>

#include "k_printf.h"

/**
 * \brief 字符串缓冲区
 *
 * k_str_buf 适用于需要频繁拼接字符串的场景，其维护了一块字符串缓冲区，
 * 支持在已有字符串基础上不断追加写入内容，并在需要时动态扩容。
 *
 * 示例：
 * ```C
 * // 初始化字符串缓冲区，指定默认的缓冲区
 * struct k_str_buf str_buf;
 * char default_buf[20];
 * k_str_buf_init(&str_buf, default_buf, sizeof(default_buf));
 *
 * // 往缓冲区中追加写入字符串
 * k_str_buf_puts(&str_buf, "hello world!");
 * k_str_buf_printf(&str_buf, "%*s", 20, "!");
 *
 * // 获取缓冲区中的字符串内容
 * char *str = k_str_buf_get(&str_buf);
 * printf("%s", str);
 *
 * // 结束使用字符串缓冲区
 * k_str_buf_free(&str_buf);
 * ```
 */
struct k_str_buf {

    /** \brief [private] 初始使用的默认缓冲区
     */
    char *default_buf;

    /** \brief [read-only] 当前正在使用的缓冲区 */
    char *str;

    /** \brief [read-only] 当前缓冲区中字符串的长度 */
    size_t str_len;

    /** \brief [private] 当前缓冲区的容量 */
    size_t capacity;
};

/**
 * \brief 初始化字符串缓冲区
 *
 * `default_buf` 指定默认缓冲区，`capacity` 为默认缓冲区的容量。
 * 若 `default_buf` 为 `NULL`，或 `capacity` 为 0 则不使用默认缓冲区。
 *
 * k_str_buf 会优先使用默认缓冲区，在容量不足时转而使用 `malloc()` 分配的新缓冲区。
 * 结束使用 k_str_buf 后，请使用 `k_str_buf_free()` 释放内存。
 */
void k_str_buf_init(struct k_str_buf *buf, char *default_buf, size_t capacity);

/** \brief 释放字符串缓冲区 */
void k_str_buf_free(struct k_str_buf *buf);

/**
 * \brief 检查缓冲区是否处于错误状态。
 *
 * 本函数用于判断缓冲区是否处于错误状态。若是，则函数返回非 0，否则返回 0。
 * 若缓冲区处于错误状态，则 `buf->buf` 为 `NULL`，且 `buf->str_len` 为 0。
 *
 * 所有向缓冲区中写入内容的函数，若执行成功则返回 0，失败返回非 0。绝大部分情况下，它们都会执行成功。
 * 只有在极端情况下，比如动态扩容时内存分配失败时，函数执行失败并返回非 0，同时将缓冲区置为错误状态。
 *
 * 一旦缓冲区被置为错误状态，后续写入操作会被快速忽略，所以不需要每步操作都要判断返回值，
 * 只需在最终获取缓冲区中字符串内容前调用本函数确认即可。
 */
int k_str_buf_failed(struct k_str_buf *buf);

/** \brief 获取缓冲区中的字符串，即 `buf->str` */
char *k_str_buf_get(struct k_str_buf *buf);

/** \brief 获取缓冲区中字符串的长度，即 `buf->str_len` */
size_t k_str_buf_get_len(struct k_str_buf *buf);

/** \brief 往缓冲区中追加写入字符串 */
int k_str_buf_puts(struct k_str_buf *buf, const char *str);

/** \brief 往缓冲区中追加写入指定长度的字符串 */
int k_str_buf_puts_n(struct k_str_buf *buf, const char *str, size_t len);

/** \brief 格式化字符串并追加写入缓冲区，格式说明符同 C printf */
int k_str_buf_printf(struct k_str_buf *buf, const char *fmt, ...);

/** \brief 格式化字符串并追加写入缓冲区，格式说明符同 C printf */
int k_str_buf_vprintf(struct k_str_buf *buf, const char *fmt, va_list args);

/** \brief 格式化字符串并追加写入缓冲区，使用 k_printf 自定义格式说明符 */
int k_str_buf_k_printf(struct k_str_buf *buf, k_printf_spec_match_fn fn_match, const char *fmt, ...);

/** \brief 格式化字符串并追加写入缓冲区，使用 k_printf 自定义格式说明符 */
int k_str_buf_k_vprintf(struct k_str_buf *buf, k_printf_spec_match_fn fn_match, const char *fmt, va_list args);

/** \brief 将缓冲区的内容重置为空字符串，但不会释放或重新分配缓冲区的内存 */
void k_str_buf_clear(struct k_str_buf *buf);

#endif
