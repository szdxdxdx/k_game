#include "./yx_all_sprite.h"

#include "../utils/sprite_sheet/yx_sprite_sheet.h"

struct k_sprite *yx_spr_iris_gun = NULL;
struct k_sprite *yx_spr_iris_bullet = NULL;

int yx_load_spr_iris(void) {

    struct yx_sprite_sheet_config config = YX_SPRITE_SHEET_CONFIG_INIT;
    config.image_filepath  = "./demo_1/sprite/iris.png";
    config.config_filepath = "./demo_1/sprite/iris.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_iris_gun,    "gun",    15, 23 },
        { &yx_spr_iris_bullet, "bullet", 16, 16 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}
