
#include "./my_room.h"

static struct k_image *img_joker;


static void my_room_2_draw(void *data) {

    static float angle = 0;
    static int flip_x = 0;
    static int flip_y = 0;

    if (k_key_down('R'))
        angle += 1.0f;

    if (k_key_pressed('X'))
        flip_x = !flip_x;

    if (k_key_pressed('Y'))
        flip_y = !flip_y;

    struct k_float_rect dst_rect;
    dst_rect.x = 10;
    dst_rect.y = 10;
    dst_rect.w = (float)k_image_get_width(img_joker);
    dst_rect.h = (float)k_image_get_height(img_joker);

    struct k_image_draw_options opts;
    opts.angle    = angle;
    opts.pivot_x  = 81;
    opts.pivot_y  = 58;
    opts.flip_x   = flip_x;
    opts.flip_y   = flip_y;

    k_image_draw(img_joker, NULL, &dst_rect, &opts);
}

static int my_room_2_init(void *params) {

    img_joker = k_image_load("./demo_1/image/joker.png");

    k_room_add_draw_callback(NULL, my_room_2_draw, 0);
    return 0;
}

struct k_room *my_room_2_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = my_room_2_init;

    return k_room_create(&config, NULL);
}
