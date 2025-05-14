#ifndef K_FONT_H
#define K_FONT_H

#include "./k_game_fwd.h"

struct k_font *k_font_load(const char *file_path, int point_size);

void k_font_release(struct k_font *font);

int k_font_set_name(struct k_font *font, const char *font_name);

struct k_font *k_font_find(const char *font_name);

/**
 * \brief 获取内置的默认字体
 *
 * 函数返回 k_game 内置的默认字体，仅支持英文、数字、少量符号。
 *
 * k_game 内置的默认字体是 ProggyVector-Regular.ttf
 * 下载地址：https://github.com/bluescan/proggyfonts
 */
struct k_font *k_font_get_builtin(void);

#endif
