
#include "k_game/core/k_image.h"

#include "image/yx_image.h"

struct k_image *img_bkgd;

struct k_image *img_ui_start_idle;
struct k_image *img_ui_start_hovered;
struct k_image *img_ui_start_pushed;

int yx_image_load_on_game_start(void) {

    img_bkgd = k_image_load("demo_1/image/bkgd.png");
    if (NULL == img_bkgd)
        return -1;

    img_ui_start_idle    = k_image_load("demo_1/image/ui_start_idle.png");
    img_ui_start_hovered = k_image_load("demo_1/image/ui_start_hovered.png");
    img_ui_start_pushed  = k_image_load("demo_1/image/ui_start_pushed.png");
    if (NULL == img_ui_start_idle || NULL == img_ui_start_hovered || NULL == img_ui_start_pushed)
        return -1;

    return 0;
}
