#include "./yx_spr.h"

#include "../utils/sprite_sheet/yx_sprite_sheet.h"

struct k_sprite *yx_spr_bubble_appear = NULL;
struct k_sprite *yx_spr_bubble_float  = NULL;
struct k_sprite *yx_spr_bubble_pop    = NULL;

int yx_load_spr_bubble(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/bubble.png";
    config.config_file_path = "./demo_1/sprite/bubble.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]) {
        { &yx_spr_bubble_appear, "appear", 15, 23 },
        { &yx_spr_bubble_float,  "float",  15, 23 },
        { &yx_spr_bubble_pop,    "pop",    15, 23 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}
