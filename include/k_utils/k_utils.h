#ifndef K_UTILS_H
#define K_UTILS_H

#include <stddef.h>

/**
 * \brief 读取文本文件，将文件所有内容写入指定缓冲区。若指定缓冲区不足，则分配新缓冲区再写入
 *
 * 函数将打开路径为 `file_path` 的文本文件，并将全篇文本内容拷贝入到缓冲区中。
 *
 * 若默认缓冲区 `buf` 的容量足够，则函数将文本拷贝到 `buf` 中，且返回值其指针。
 *
 * 否则，函数使用 `malloc` 分配新的缓冲区，然后再次拷贝文本到新的缓冲区，并返回其指针。
 * 判断函数返回的缓冲区指针是否等于入参 `buf`，即可知是否分配了新的缓冲区。
 * 若函数分配了新的缓冲区，新缓冲区最终需由用户手动使用 `free` 释放。
 *
 * 若指定默认缓冲区 `buf` 为 `NULL` 或其容量 `buf_size` 为 0，则函数将一定分配新缓冲区。
 *
 * 出参 `get_len` 是可选的，若不为 `NULL`，则返回文本的长度，单位：字节，而非字符。
 *
 * 若读取成功，函数返回缓冲区指针，否则返回 `NULL`。
 */
char *k_read_txt_file(const char *file_path, char *buf, size_t buf_size, size_t *get_len);

#endif
