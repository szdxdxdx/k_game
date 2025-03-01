#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

static struct k_image *img;
static struct k_sprite *spr = NULL;
static uint64_t spr_timer = 0;
static size_t spr_frame_idx = 0;
static float x = 30.0f;
static float y = 30.0f;

static void room_step(void *data) {

    float delta_ms = k_get_step_delta();

    if (k_is_key_down('W'))
        y -= 100.0f * delta_ms;
    if (k_is_key_down('A'))
        x -= 100.0f * delta_ms;
    if (k_is_key_down('D'))
        x += 100.0f * delta_ms;
    if (k_is_key_down('S'))
        y += 100.0f * delta_ms;
}

static void room_draw(void *data) {

    k_draw_sprite_frame(spr, (int)x, (int)y, spr_frame_idx);

    if (spr_timer <= k_get_step_timestamp()) {
        spr_timer += 150;

        spr_frame_idx += 1;
        spr_frame_idx %= 4;
    }
}

static int init_game(void) {

    img = k_load_image(NULL, "assets/tmp.png");

    int sprite_w = 512 / 8;
    int sprite_h = 256 / 3;

    spr = k_create_sprite((struct k_sprite_config[1]) {{
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

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    room_config.room_name = "room_1";
    struct k_room *room = k_create_room(&room_config, NULL);
    k_room_add_step_callback(room, room_step, NULL);
    k_room_add_draw_callback(room, room_draw, NULL, 0);

    k_goto_room(room);
    return 0;
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config game_config = K_GAME_CONFIG_INIT;
    game_config.fn_init = init_game;
    k_run_game(&game_config);

    return 0;
}

#endif
