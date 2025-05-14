#include "./yx_spr.h"

#include "../utils/sprite_sheet/yx_sprite_sheet.h"

struct k_sprite *yx_spr_iris_gun    = NULL;
struct k_sprite *yx_spr_iris_bullet = NULL;
struct k_sprite *yx_spr_iris_apple  = NULL;

int yx_spr_iris_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/iris.png";
    config.config_file_path = "./demo_1/sprite/iris.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_iris_gun,    "gun",    15, 23 },
        { &yx_spr_iris_bullet, "bullet", 16, 16 },
        { &yx_spr_iris_apple,  "apple", 16, 16 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}
