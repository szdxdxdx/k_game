#include "./demo_1.h"

static void my_load_sprite_player(void) {

    struct k_image *img_player = k_image_load("./demo_1/mechanic_mike.png");
    k_image_set_name(img_player, "player");

    int sprite_w = 80;
    int sprite_h = 64;
    float origin_x = 40.0f;
    float origin_y = 43.0f;

    {
        int frame_delay = 120;

        struct k_sprite_config config = {
            .sprite_w = sprite_w,
            .sprite_h = sprite_h,
            .origin_x = origin_x,
            .origin_y = origin_y,
            .frames = (struct k_sprite_frame_config[]){
                { img_player, 0 * sprite_w, 0, frame_delay },
                { img_player, 1 * sprite_w, 0, frame_delay },
                { img_player, 2 * sprite_w, 0, frame_delay },
                { img_player, 3 * sprite_w, 0, frame_delay },
                { img_player, 4 * sprite_w, 0, frame_delay },
                { img_player, 5 * sprite_w, 0, frame_delay },
                { img_player, 6 * sprite_w, 0, frame_delay },
            },
            .frames_num = 7
        };
        struct k_sprite *sprite = k_sprite_create(&config);
        k_sprite_set_name(sprite, "player/idle");
    }

    {
        int frame_delay = 120;
        struct k_sprite_config config = {
            .sprite_w = sprite_w,
            .sprite_h = sprite_h,
            .origin_x = origin_x,
            .origin_y = origin_y,
            .frames = (struct k_sprite_frame_config[]){
                { img_player,  7 * sprite_w, 0, frame_delay },
                { img_player,  8 * sprite_w, 0, frame_delay },
                { img_player,  9 * sprite_w, 0, frame_delay },
                { img_player, 10 * sprite_w, 0, frame_delay },
                { img_player, 11 * sprite_w, 0, frame_delay },
                { img_player, 12 * sprite_w, 0, frame_delay },
            },
            .frames_num = 6
        };
        struct k_sprite *sprite = k_sprite_create(&config);
        k_sprite_set_name(sprite, "player/run");
    }
}

static void my_load_sprite_enemy(void) {

    struct k_image *img_enemy = k_image_load("./demo_1/microwave.png");
    k_image_set_name(img_enemy, "enemy");
}

void my_load_sprite_asset(void) {

    my_load_sprite_player();
    my_load_sprite_enemy();
}
