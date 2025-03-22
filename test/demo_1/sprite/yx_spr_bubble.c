#include "./_internal.h"

struct k_sprite *yx_spr_bubble_appearing = NULL;
struct k_sprite *yx_spr_bubble_floating  = NULL;
struct k_sprite *yx_spr_bubble_popped    = NULL;

static struct k_sprite *load_sprite_strip(const char *filepath, int frames_num, int delay) {

    int scale = 2;

    struct k_image *img = k_image_load(filepath);
    int scaled_image_w = scale * k_image_get_width(img);
    int scaled_image_h = scale * k_image_get_height(img);
    struct k_image *img_player = k_image_scale(img, scaled_image_w, scaled_image_h);
    k_image_release(img);

    int sprite_w = k_image_get_width(img_player) / frames_num;
    int sprite_h = k_image_get_height(img_player);

    float origin_x = (float)sprite_w / 2;
    float origin_y = (float)sprite_h / 3 * 2;

    /* 素材中的动画都不超过 15 帧，此处不需要动态申请内存 */
    struct k_sprite_frame_config frames_config[] = {
            { img_player,  0 * sprite_w, 0, delay },
            { img_player,  1 * sprite_w, 0, delay },
            { img_player,  2 * sprite_w, 0, delay },
            { img_player,  3 * sprite_w, 0, delay },
            { img_player,  4 * sprite_w, 0, delay },
            { img_player,  5 * sprite_w, 0, delay },
            { img_player,  6 * sprite_w, 0, delay },
            { img_player,  7 * sprite_w, 0, delay },
            { img_player,  8 * sprite_w, 0, delay },
            { img_player,  9 * sprite_w, 0, delay },
            { img_player, 10 * sprite_w, 0, delay },
            { img_player, 11 * sprite_w, 0, delay },
            { img_player, 12 * sprite_w, 0, delay },
            { img_player, 13 * sprite_w, 0, delay },
            { img_player, 14 * sprite_w, 0, delay },
            { img_player, 15 * sprite_w, 0, delay },
    };

    struct k_sprite_config config = {
        .sprite_w   = sprite_w,
        .sprite_h   = sprite_h,
        .origin_x   = origin_x,
        .origin_y   = origin_y,
        .frames     = frames_config,
        .frames_num = frames_num
    };

    return k_sprite_create(&config);
}

int yx_load_spr_bubble(void) {

    yx_spr_bubble_appearing = load_sprite_strip("./demo_1/sprite/bubble/appearing.png", 11, 64);
    yx_spr_bubble_floating  = load_sprite_strip("./demo_1/sprite/bubble/floating.png",  4, 192);
    yx_spr_bubble_popped    = load_sprite_strip("./demo_1/sprite/bubble/popped.png",    5,  64);
    return 0;
}
