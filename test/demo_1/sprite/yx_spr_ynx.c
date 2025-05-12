#include "./yx_spr.h"

#include "../utils/sprite_sheet/yx_sprite_sheet.h"

struct k_sprite *yx_spr_ynx_idle = NULL;
struct k_sprite *yx_spr_ynx_run  = NULL;

int yx_spr_ynx_load(void) {
    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/ynx.png";  /* aseprite 导出的精灵表 */
    config.config_file_path = "./demo_1/sprite/ynx.json"; /* aseprite 导出的精灵配置 */
    config.scale = 2.0f;                                  /* 缩放原素材图片 */
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_ynx_idle, "idle", 15, 23 }, /* 从精灵表中加载标签为 idle 的精灵 */
        { &yx_spr_ynx_run,  "run",  15, 23 }, /* 从精灵表中加载标签为 run  的精灵 */
        { NULL }
    };

    yx_sprite_load_from_sheet(&config); /* 加载精灵 */

    return yx_sprite_load_from_sheet(&config);
}
