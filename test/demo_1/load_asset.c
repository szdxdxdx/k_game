#include "./demo_1.h"

static struct k_sprite *my_load_sprite_strip(const char *filepath, int frames_num, const char *sprite_name) {

    int scale = 2;

    int sprite_w = scale * 80;
    int sprite_h = scale * 64;
    float origin_x = (float)scale * 27.0f;
    float origin_y = (float)scale * 32.0f;

    struct k_image *img = k_image_load(filepath);

    int scaled_w = scale * k_image_get_width(img);
    int scaled_h = scale * k_image_get_height(img);

    struct k_image *img_player = k_image_scale(img, scaled_w, scaled_h);
    k_image_release(img);

    /* 素材中的动画都不超过 15 帧，此处不需要动态申请内存 */
    struct k_sprite_frame_config frames_config[] = {
            { img_player,  0 * sprite_w, 0, 90 },
            { img_player,  1 * sprite_w, 0, 90 },
            { img_player,  2 * sprite_w, 0, 90 },
            { img_player,  3 * sprite_w, 0, 90 },
            { img_player,  4 * sprite_w, 0, 90 },
            { img_player,  5 * sprite_w, 0, 90 },
            { img_player,  6 * sprite_w, 0, 90 },
            { img_player,  7 * sprite_w, 0, 90 },
            { img_player,  8 * sprite_w, 0, 90 },
            { img_player,  9 * sprite_w, 0, 90 },
            { img_player, 10 * sprite_w, 0, 90 },
            { img_player, 11 * sprite_w, 0, 90 },
            { img_player, 12 * sprite_w, 0, 90 },
            { img_player, 13 * sprite_w, 0, 90 },
            { img_player, 14 * sprite_w, 0, 90 },
            { img_player, 15 * sprite_w, 0, 90 },
    };

    struct k_sprite_config config = {
        .sprite_w   = sprite_w,
        .sprite_h   = sprite_h,
        .origin_x   = origin_x,
        .origin_y   = origin_y,
        .frames     = frames_config,
        .frames_num = frames_num
    };
    struct k_sprite *sprite = k_sprite_create(&config);
    k_sprite_set_name(sprite, sprite_name);

    return sprite;
}

static void my_load_sprite_player(void) {

    my_load_sprite_strip("./demo_1/player/run.png",  6, "player/run");
    my_load_sprite_strip("./demo_1/player/idle.png", 7, "player/idle");
}

static void my_load_sprite_enemy(void) {

    my_load_sprite_strip("./demo_1/enemy/Attack.png", 6, "enemy/attack");

}

void my_load_sprite_asset(void) {

    my_load_sprite_player();
    my_load_sprite_enemy();
}
