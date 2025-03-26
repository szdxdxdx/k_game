#ifndef YX_SPRITE_SHEET_H
#define YX_SPRITE_SHEET_H

#include "k_game.h"

struct yx_sprite_sheet_sprite_config {

    struct k_sprite **get_sprite;

    const char *tag;

    float origin_x;
    float origin_y;
};

struct yx_sprite_sheet_config {

    const char *image_filepath;

    const char *config_filepath;

    float scale;

    /**
     * \brief 精灵
     *
     * 指向精灵配置数组的指针，用于定义精灵表中的每一个精灵。
     * 数组的最后一项必须是哨兵值 `{ NULL }`
     */
    struct yx_sprite_sheet_sprite_config *sprites;
};

#define YX_SPRITE_SHEET_CONFIG_INIT \
{ \
    .image_filepath  = NULL, \
    .config_filepath = NULL, \
    .scale           = 1.0f, \
    .sprites         = NULL, \
}

/**
 * \brief 从 aseprite 导出的精灵表中加载精灵
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int yx_sprite_load_from_sheet(const struct yx_sprite_sheet_config *config);

#endif
