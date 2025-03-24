#include "./_internal.h"

struct k_sprite *yx_spr_ynx_idle = NULL;
struct k_sprite *yx_spr_ynx_run  = NULL;

int yx_load_spr_ynx(void) {

    struct yx_sprite_sheet sheet;

    const char *config = "./demo_1/sprite/ynx.json";
    const char *image  = "./demo_1/sprite/ynx.png";
    yx_load_sprite_sheet(&sheet, image, 2.0f, config);

    yx_spr_ynx_idle = yx_load_sprite_from_sheet(&sheet, "idle", 15, 23);
    yx_spr_ynx_run = yx_load_sprite_from_sheet(&sheet, "run", 15, 23);

    yx_sprite_sheet_fini(&sheet);
    return 0;
}
