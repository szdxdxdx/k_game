#include "./demo_1.h"

static struct k_sprite *my_load_sprite_strip(const char *filepath, int frames_num, int frame_delay) {

    struct k_image *img = k_image_load(filepath);



}

static void my_load_sprite_player(void) {

    int scale = 2;

    int   sprite_w = scale * 80;
    int   sprite_h = scale * 64;
    float origin_x = (float)scale * 40.0f;
    float origin_y = (float)scale * 43.0f;

    {
        struct k_image *img = k_image_load("./demo_1/player/run.png");

        int scaled_w = scale * k_image_get_width(img);
        int scaled_h = scale * k_image_get_height(img);

        struct k_image *img_player = k_image_scale(img, scaled_w, scaled_h);
        k_image_release(img);

        struct k_sprite_config config = {
            .sprite_w = sprite_w,
            .sprite_h = sprite_h,
            .origin_x = origin_x,
            .origin_y = origin_y,
            .frames = (struct k_sprite_frame_config[]){
                { img_player, 0 * sprite_w, 0, 150 },
                { img_player, 1 * sprite_w, 0, 150 },
                { img_player, 2 * sprite_w, 0, 150 },
                { img_player, 3 * sprite_w, 0, 150 },
                { img_player, 4 * sprite_w, 0, 150 },
                { img_player, 5 * sprite_w, 0, 150 },
            },
            .frames_num = 6
        };
        struct k_sprite *sprite = k_sprite_create(&config);
        k_sprite_set_name(sprite, "player/run");
    }
}

static void my_load_sprite_enemy(void) {

}

void my_load_sprite_asset(void) {

    my_load_sprite_player();
    my_load_sprite_enemy();
}
