#ifndef K_CONTAINER_OF_H
#define K_CONTAINER_OF_H

#include <stddef.h>

#ifndef offsetof

/**
 * \brief 获取结构体成员相对于结构体起始地址的偏移量（单位：字节）
 *
 * 示例：
 * ```c
 * struct example {
 *     int a;
 *     int b;
 * };
 *
 * size_t offset = offsetof(struct example, b);
 * // offset 通常为 4（取决于平台和对齐规则）
 * ```
 */
#define offsetof(type, member) \
    ((size_t)&(((type *)0)->member))

#endif

#ifndef container_of

/**
 * \brief 通过指向结构体成员的指针，反推出指向整个结构体的指针
 *
 * `p` 为指向结构体成员的指针，`type` 为该成员所在的结构体的类型，
 * `member` 为该成员在结构体中的名字。
 *
 * 必须保证传入的 `p` 正确指向结构体中 `member` 成员。
 *
 * 示例：
 * ```C
 * struct example {
 *     int a;
 *     int b;
 * };
 *
 * // 指向结构体成员 `b` 的指针
 * int *p_b = ...;
 *
 * // 通过指向结构体成员 `b` 的指针，反推出指向整个结构体 `example` 的指针
 * struct example *p_example = container_of(p_b, struct example, b);
 * ```
 */
#define container_of(p, type, member) \
    ((type *)((char *)(p) - offsetof(type, member)))

#endif

#endif
