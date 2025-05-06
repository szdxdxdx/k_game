#include "./yx_spr.h"

#include "../utils/sprite_sheet/yx_sprite_sheet.h"

struct k_sprite *yx_spr_liliko_idle = NULL;
struct k_sprite *yx_spr_liliko_run  = NULL;

int yx_spr_liliko_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/liliko.png";
    config.config_file_path = "./demo_1/sprite/liliko.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_liliko_idle, "idle", 15, 23 },
        { &yx_spr_liliko_run,  "run",  15, 23 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}
