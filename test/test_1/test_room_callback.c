
#if 1

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

static struct k_sprite *spr_player;

struct obj_player {
    float x, y;
};

static void object_draw_1(struct k_object *object)  { printf(" 1 "); }
static void object_draw_2(struct k_object *object)  { printf(" 2 "); }
static void object_draw_3(struct k_object *object)  { printf(" 3 "); }
static void object_draw_4(struct k_object *object)  { printf(" 4 "); }
static void object_draw_5(struct k_object *object)  { printf(" 5 "); }
static void object_draw_6(struct k_object *object)  { printf(" 6 "); }
static void object_draw_7(struct k_object *object)  { printf(" 7 "); }
static void object_draw_8(struct k_object *object)  { printf(" 8 "); }
static void object_draw_9(struct k_object *object)  { printf(" 9 "); }

static void object_draw_end_line(struct k_object *object)  { printf("\n"); }

static void object_draw_11(struct k_object *object) { printf("11 "); }
static void object_draw_12(struct k_object *object) { printf("12 "); }
static void object_draw_13(struct k_object *object) { printf("13 "); }
static void object_draw_14(struct k_object *object) { printf("14 "); }
static void object_draw_15(struct k_object *object) { printf("15 "); }
static void object_draw_16(struct k_object *object) { printf("16 "); }
static void object_draw_17(struct k_object *object) { printf("17 "); }
static void object_draw_18(struct k_object *object) { printf("18 "); }
static void object_draw_19(struct k_object *object) { printf("19 "); }

static void object_draw_21(struct k_object *object) { printf("21 "); }
static void object_draw_22(struct k_object *object) { printf("22 "); }
static void object_draw_23(struct k_object *object) { printf("23 "); }
static void object_draw_24(struct k_object *object) { printf("24 "); }
static void object_draw_25(struct k_object *object) { printf("25 "); }
static void object_draw_26(struct k_object *object) { printf("26 "); }
static void object_draw_27(struct k_object *object) { printf("27 "); }
static void object_draw_28(struct k_object *object) { printf("28 "); }
static void object_draw_29(struct k_object *object) { printf("29 "); }

static void object_step(struct k_object *object) {
    struct obj_player *player = k_object_get_data(object);

    if (k_key_pressed('1')) {

        k_object_add_draw_callback(object, object_draw_1, 0);
        k_object_add_draw_callback(object, object_draw_2, 0);
        k_object_add_draw_callback(object, object_draw_3, 0);
        k_object_add_draw_callback(object, object_draw_4, 0);
        k_object_add_draw_callback(object, object_draw_5, 0);

        k_object_add_draw_callback(object, object_draw_end_line, 5);

        k_object_add_draw_callback(object, object_draw_11, 10);
        k_object_add_draw_callback(object, object_draw_12, 10);

        k_object_add_draw_callback(object, object_draw_6, 0);
        k_object_add_draw_callback(object, object_draw_7, 0);
        k_object_add_draw_callback(object, object_draw_8, 0);
        k_object_add_draw_callback(object, object_draw_9, 0);
        k_object_add_draw_callback(object, object_draw_21, 20);
        k_object_add_draw_callback(object, object_draw_22, 20);
        k_object_add_draw_callback(object, object_draw_23, 20);

        k_object_add_draw_callback(object, object_draw_end_line, 25);

        k_object_add_draw_callback(object, object_draw_24, 20);
        k_object_add_draw_callback(object, object_draw_25, 20);
        k_object_add_draw_callback(object, object_draw_26, 20);
        k_object_add_draw_callback(object, object_draw_27, 20);

        k_object_add_draw_callback(object, object_draw_13, 10);
        k_object_add_draw_callback(object, object_draw_14, 10);
        k_object_add_draw_callback(object, object_draw_15, 10);
        k_object_add_draw_callback(object, object_draw_16, 10);

        k_object_add_draw_callback(object, object_draw_end_line, 15);
        k_object_add_draw_callback(object, object_draw_28, 20);
        k_object_add_draw_callback(object, object_draw_29, 20);

        k_object_add_draw_callback(object, object_draw_17, 10);
        k_object_add_draw_callback(object, object_draw_18, 10);
        k_object_add_draw_callback(object, object_draw_19, 10);
    }

    if (k_key_pressed('A')) {
        k_object_del_all_callbacks(object);
        k_object_add_step_callback(object, object_step);
    }
}

static int create_room(struct k_room *room, void *params) {

    struct k_object *object = k_object_create(sizeof(struct obj_player));
    struct obj_player *player = k_object_get_data(object);
    player->x = 0.0f;
    player->y = 0.0f;

    k_object_add_step_callback(object, object_step);

    /* ------------------------------------------------------------------------ */

    struct k_component_type *renderer = k_component_type_find("k/spr-rdr");
    struct k_sprite_renderer_config renderer_config;
    renderer_config.sprite  = spr_player;
    renderer_config.z_index = 0;
    renderer_config.x       = &player->x;
    renderer_config.y       = &player->y;
    k_object_add_component(object, renderer, &renderer_config);

    /* ------------------------------------------------------------------------ */

    struct k_component_type *WASD = k_component_type_find("k/WASD");
    struct k_WASD_config WASD_config;
    WASD_config.key_up    = 'W';
    WASD_config.key_left  = 'A';
    WASD_config.key_down  = 'S';
    WASD_config.key_right = 'D';
    WASD_config.speed     = 200.0f;
    WASD_config.x         = &player->x;
    WASD_config.y         = &player->y;
    k_object_add_component(object, WASD, &WASD_config);

    return 0;
}

static int init(void) {

    struct k_image *img = k_image_load("1", "assets/tmp.png");

    /* ------------------------------------------------------------------------ */

    int sprite_w = 512 / 8;
    int sprite_h = 256 / 3;

    spr_player = k_sprite_create((struct k_sprite_config[1]) {{
        .sprite_name = NULL,
        .sprite_w = sprite_w,
        .sprite_h = sprite_h,
        .origin_x = 30,
        .origin_y = 30,
        .frames = (struct k_sprite_frame_config[]) {
            { .image = img, .offset_x = 0 * sprite_w, .offset_y = 0, .delay = 200 },
            { .image = img, .offset_x = 1 * sprite_w, .offset_y = 0, .delay = 200 },
            { .image = img, .offset_x = 2 * sprite_w, .offset_y = 0, .delay = 200 },
            { .image = img, .offset_x = 3 * sprite_w, .offset_y = 0, .delay = 200 },
        },
        .frames_num = 4
    }});

    /* ------------------------------------------------------------------------ */

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    room_config.fn_init = create_room;
    room_config.room_speed = 30;

    struct k_room *room = k_room_create(&room_config, NULL);

    /* ------------------------------------------------------------------------ */

    k_goto_room(room);

    return 0;
}

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config game_config = K_GAME_CONFIG_INIT;
    game_config.window_w = 400;
    game_config.window_h = 360;
    game_config.fn_init = init;

    k_game_run(&game_config);

    return 0;
}

#endif
