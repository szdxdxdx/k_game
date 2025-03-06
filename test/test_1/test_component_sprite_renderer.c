#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

static struct k_sprite *spr_player;

struct obj_player {
    float x, y;
};

static int create_room(struct k_room *room, void *params) {

    struct k_object *object = k_object_create(room, sizeof(struct obj_player));
    struct obj_player *player = k_object_get_data(object);
    player->x = 0.0f;
    player->y = 0.0f;

    /* ------------------------------------------------------------------------ */

    struct k_component_type *renderer = k_component_find("k/spr-rdr");
    struct k_sprite_renderer_config renderer_config;
    renderer_config.sprite  = spr_player;
    renderer_config.z_index = 0;
    renderer_config.x       = &player->x;
    renderer_config.y       = &player->y;
    k_object_add_component(object, renderer, &renderer_config);

    /* ------------------------------------------------------------------------ */

    struct k_component_type *WASD = k_component_find("k/WASD");
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

    struct k_room *room = k_room_create(&room_config, NULL);

    /* ------------------------------------------------------------------------ */

    k_goto_room(room);

    return 0;
}

#if 1

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
