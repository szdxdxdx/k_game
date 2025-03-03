#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

/* region [img] */

static struct k_image *img;

/* endregion */

/* region [obj_player] */

struct obj_player {
    struct k_component *WASD;
    uint64_t spr_timer;
    int current_frame;
};

static void object_draw(struct k_object *object) {
    struct obj_player *player = k_object_get_data(object);

    int w = 512 / 8;
    int h = 256 / 3;

    struct k_int_rect rect;
    rect.x = (player->current_frame + 4) * w;
    rect.y = 0;
    rect.w = w;
    rect.h = h;

    struct k_component_WASD *position = k_component_get_data(player->WASD);
    k_draw_image(img, &rect, (int)position->x, (int)position->y);

    uint64_t current_ms = k_get_step_timestamp();
    if (player->spr_timer <= current_ms) {
        player->spr_timer += 150;

        player->current_frame += 1;
        player->current_frame %= 4;
    }
}

/* endregion */

/* region [game && room] */

static int create_room(struct k_room *room, void *params) {

    struct k_object *object = k_create_object(sizeof(struct obj_player));
    struct obj_player *player = k_object_get_data(object);
    player->spr_timer = 0;
    player->current_frame = 0;

    k_object_add_draw_callback(object, object_draw, 0);

    struct k_component_type *WASD = k_get_component_type_by_name("WASD");
    struct k_component_WASD_config WASD_config;
    WASD_config.key_up    = 'W';
    WASD_config.key_down  = 'S';
    WASD_config.key_left  = 'A';
    WASD_config.key_right = 'D';
    WASD_config.speed = 200.0f;
    WASD_config.x = 30.f;
    WASD_config.y = 30.f;
    player->WASD = k_object_add_component(object, WASD, &WASD_config);

    return 0;
}

static int init(void) {

    img = k_load_image("", "assets/tmp.png");
    (void)img;

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    room_config.fn_init = create_room;
    struct k_room *room = k_create_room(&room_config, NULL);

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

    k_run_game(&game_config);

    return 0;
}

/* endregion */

#endif
