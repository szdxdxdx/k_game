#include "./_internal.h"

struct k_sprite *yx_spr_bubble_appear = NULL;
struct k_sprite *yx_spr_bubble_float   = NULL;
struct k_sprite *yx_spr_bubble_pop    = NULL;

int yx_load_spr_bubble(void) {

    struct yx_sprite_sheet sheet;

    const char *config = "./demo_1/sprite/bubble.json";
    const char *image  = "./demo_1/sprite/bubble.png";
    yx_load_sprite_sheet(&sheet, image, 2.0f, config);

    yx_spr_bubble_appear = yx_load_sprite_from_sheet(&sheet, "appear", 15, 23);
    yx_spr_bubble_float  = yx_load_sprite_from_sheet(&sheet, "float", 15, 23);
    yx_spr_bubble_pop    = yx_load_sprite_from_sheet(&sheet, "pop", 15, 23);

    yx_sprite_sheet_fini(&sheet);
    return 0;
}
