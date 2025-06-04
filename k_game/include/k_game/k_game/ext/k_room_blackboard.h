#ifndef K_ROOM_BLACKBOARD_H
#define K_ROOM_BLACKBOARD_H

#include <stddef.h>

/**
 * \brief 房间黑板
 *
 * 黑板是一个房间内的全局哈希表，键为 char * 字符串类型，值为任意类型。
 *
 * k_room_blackboard 的本质是 k_component_manager。
 */
struct k_room_blackboard;

/**
 * \brief 给房间添加一个黑板
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_room_add_blackboard(void);

/**
 * \brief 往黑板中添加一个新的键值对
 *
 * 若键不存在，则添加新键值对。若键已存在，则添加失败。
 *
 * `key` 是要添加的键，要求传入有效的字符串指针，
 * 黑板会复制保存 `key` 字符串。
 *
 * `val_size` 指定要添加的值的大小，单位：字节。
 * 要求 `val_size` 大于 0。
 *
 * 若添加成功，函数返回指向用于存储新值的内存段的指针，
 * 你需要显式类型转换该指针，然后写入新值。若添加失败，函数返回 `NULL`。
 */
void *k_room_blackboard_add(const char *key, size_t value_size);

/**
 * \brief 往黑板中添加或更新一个键值对
 *
 * 若键不存在，则添加新键值对。
 * 若键已存在，则释放旧值的内存，然后分配新内存来存储新值。
 *
 * `key` 是要添加或更新的键，要求传入有效的字符串指针，
 * 黑板会复制保存 `key` 字符串。
 *
 * `val_size` 指定要添加或更新的值的大小，单位：字节。
 * 要求 `val_size` 大于 0。
 *
 * 若添加或更新成功，函数返回指向用于存储新值的内存段的指针，
 * 你需要显式类型转换该指针，然后写入新值。若添加失败，函数返回 `NULL`。
 */
void *k_room_blackboard_put(const char *key, size_t value_size);

/**
 * \brief 获取黑板中指定键对应的值
 *
 * 函数返回 `key` 对应的值的指针，若 `key` 不存在则返回 `NULL`。
 */
void *k_room_blackboard_get(const char *key);

/**
 * \brief 删除黑板中的一个键值对
 *
 * 若 `key` 不存在，则函数立即返回。
 */
void k_room_blackboard_remove(const char *key);

#endif
