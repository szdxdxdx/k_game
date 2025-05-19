
#include "k_game/core/k_image.h"

#include "image/yx_image.h"

struct k_image *img_bkgd;

int yx_image_load_on_game_start(void) {

    img_bkgd = k_image_load("demo_1/image/bkgd.png");
    if (NULL == img_bkgd)
        return -1;

    return 0;
}
