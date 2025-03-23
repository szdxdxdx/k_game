#include "./_internal.h"

struct k_sprite *yx_spr_liliko_idle = NULL;
struct k_sprite *yx_spr_liliko_run  = NULL;

int yx_load_spr_liliko(void) {

    struct yx_sprite_sheet sheet;

    const char *config = "./demo_1/sprite/liliko.json";
    const char *image  = "./demo_1/sprite/liliko.png";
    yx_sprite_sheet_init(&sheet, image, 2.0f, config);

    yx_spr_liliko_idle = yx_sprite_load_from_sheet(&sheet, "idle", 15, 23);
    yx_spr_liliko_run = yx_sprite_load_from_sheet(&sheet, "run", 15, 23);

    yx_sprite_sheet_fini(&sheet);
    return 0;
}
