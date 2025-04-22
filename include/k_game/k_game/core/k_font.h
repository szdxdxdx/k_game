#ifndef K_FONT_H
#define K_FONT_H

#include "./k_game_fwd.h"

struct k_font *k_font_load(const char *file_path, int font_size);

void k_font_release(struct k_font *font);

/**
 * \brief 设置字体的名字
 *
 * 字体名字是可选的，默认情况下字体没有名字。
 *
 * 你可以为字体设置唯一名字，k_game 将基于该名字为字体建立索引，
 * 之后可使用 `k_sprite_find()` 根据名字查找字体。
 *
 * 若名字设为空字符串 "" 或 `NULL`，则清除名字，并删除索引。
 *
 * 注意：k_game 不会复制名字，而仅是保存字符串指针。
 * 请确保该字符串的内存始终有效，且内容不被修改。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_font_set_name(struct k_font *font, const char *font_name);

/**
 * \brief 通过字体名称查找字体
 *
 * 若找到，函数返回字体的指针，否则返回 `NULL`。
 */
struct k_font *k_font_find(const char *font_name);

#endif
