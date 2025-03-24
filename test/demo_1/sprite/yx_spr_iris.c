#include "./_internal.h"

struct k_sprite *yx_spr_iris_gun = NULL;
struct k_sprite *yx_spr_iris_bullet = NULL;

int yx_load_spr_iris(void) {

    struct yx_sprite_sheet sheet;

    const char *config = "./demo_1/sprite/iris.json";
    const char *image  = "./demo_1/sprite/iris.png";
    yx_load_sprite_sheet(&sheet, image, 2.0f, config);

    yx_spr_iris_gun = yx_load_sprite_from_sheet(&sheet, "gun", 15, 23);
    yx_spr_iris_bullet  = yx_load_sprite_from_sheet(&sheet, "bullet", 16, 16);

    yx_sprite_sheet_fini(&sheet);
    return 0;
}
