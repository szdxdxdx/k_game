#ifndef YX_SPRITE_SHEET_H
#define YX_SPRITE_SHEET_H

#include "k_game/core/k_game_fwd.h"

struct yx_sprite_sheet_config;

/**
 * \brief 从 aseprite 导出的精灵表中加载精灵
 *
 * aseprite 是专门用来绘制像素画的软件，可以用它来绘制像素游戏素材，
 * 支持导出素材精灵表 .png 和配置 .json。
 *
 * 注意：本函数使用 k_json 模块解析配置，而 k_json 模块尚未完善，
 * 不一定能正确解析 json 文本。所以本函数也是处于试验阶段。
 *
 * 本函数将根据配置，从精灵表图片中加载精灵。
 * 若加载成功，函数返回 0，否则返回非 0。
 */
int yx_sprite_load_from_sheet(const struct yx_sprite_sheet_config *config);

/** \brief 用于定义 aseprite 精灵表中精灵的配置项 */
struct yx_sprite_sheet_sprite_config {

    /** \brief 出参，请传入合法的地址，用于获取从精灵表中提取出的精灵 */
    struct k_sprite **get_sprite;

    /** \brief 精灵的标签 */
    const char *tag;

    /** \brief 精灵原点 */
    float origin_x;
    float origin_y;
};

/**
 * \brief 用于加载 aseprite 导出的精灵表的配置
 *
 * 请按如下要求精灵表：
 * - 【布局】表类型选择按行，不要合并重复。
 * - 【精灵】来源选择 sprite，选择选择所有帧，且选拆分标签。
 * - 【边界】所有数值填 0，不要勾选任何修剪，不要拉伸，
 * - 【输出】勾选导出 png 和 json，元信息选择数组，并勾选标签。
 */
struct yx_sprite_sheet_config {

    /** \brief 精灵表图片 .png 文件的路径 */
    const char *image_filepath;

    /** \brief 精灵表数据 .json 文件的路径 */
    const char *config_filepath;

    /**
     * \brief 对精灵表的缩放倍率
     *
     * 该参数可以放大或缩小整张精灵表图片的尺寸，
     * 载入的精灵也会随着放大或缩小，
     * 精灵原点的位置会也自动按比例缩放。
     */
    float scale;

    /**
     * \brief 精灵
     *
     * 指向精灵配置数组的指针，用于定义精灵表中的精灵。
     * 数组最后一项必须是哨兵值 `{ .get_sprite = NULL }`，或者简写成 `{ NULL }`。
     */
    struct yx_sprite_sheet_sprite_config *sprites;
};

/** \brief 用于加载 aseprite 导出的精灵表的配置默认值 */
#define YX_SPRITE_SHEET_CONFIG_INIT \
{ \
    .image_filepath  = NULL, \
    .config_filepath = NULL, \
    .scale           = 1.0f, \
    .sprites         = NULL, \
}

#endif
