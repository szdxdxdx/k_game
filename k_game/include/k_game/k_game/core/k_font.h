#ifndef K_FONT_H
#define K_FONT_H

#include "./k_game_fwd.h"

/**
 * \brief 加载字体资源
 *
 * 目前支持加载 .ttf 格式的字体。
 *
 * `point_size` 指定字体的磅数。
 * 若加载成功，函数返回字体的指针，否则返回 `NULL`。
 */
struct k_font *k_font_load(const char *file_path, int point_size);

/**
 * \brief 释放字体资源
 *
 * 游戏结束时 k_game 会释放字体资源，无需手动释放。
 */
void k_font_release(struct k_font *font);

/**
 * \brief 获取内置的默认字体
 *
 * 函数返回 k_game 内置的默认字体，仅支持英文、数字和少量符号。
 *
 * k_game 内置的默认字体是 ProggyVector-Regular.ttf
 * 下载地址：https://github.com/bluescan/proggyfonts
 */
struct k_font *k_font_get_builtin(void);

#endif
