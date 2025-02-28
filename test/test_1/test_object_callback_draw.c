#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

/* region [img] */

static struct k_image *img;
static struct k_object *players[3];

/* endregion */

/* region [obj_player] */

struct obj_player {
    float x;
    float y;

    uint64_t spr_timer;
    int current_frame;
};

static void object_step(struct k_object *object) {
    struct obj_player *player = k_object_get_data(object);
    float delta_ms = k_get_step_delta();
    float speed = 100.0f;

    if (k_is_key_down('W'))
        player->y -= speed * delta_ms;
    if (k_is_key_down('A'))
        player->x -= speed * delta_ms;
    if (k_is_key_down('D'))
        player->x += speed * delta_ms;
    if (k_is_key_down('S'))
        player->y += speed * delta_ms;
}

static void object_draw(struct k_object *object) {
    struct obj_player *player = k_object_get_data(object);

    int w = 512 / 8;
    int h = 256 / 3;

    struct k_int_rect rect;
    rect.x = (player->current_frame + 4) * w;
    rect.y = 0;
    rect.w = w;
    rect.h = h;

    k_draw_image(img, &rect, (int)player->x, (int)player->y);

    uint64_t current_ms = k_get_step_timestamp();
    if (player->spr_timer <= current_ms) {
        player->spr_timer += 150;

        player->current_frame += 1;
        player->current_frame %= 4;
    }
}

static struct k_object *create_player(float x, float y, int z_index) {

    struct k_object *object = k_create_object(sizeof(struct obj_player));
    if (NULL == object)
        return NULL;

    struct obj_player *player = k_object_get_data(object);
    player->x = x;
    player->y = y;
    player->spr_timer = 0;
    player->current_frame = 0;

    k_object_add_step_callback(object, object_step);
    k_object_add_draw_callback(object, object_draw, z_index);

    return object;
}

static void destroy_player(struct k_object *obj_player) {

    if (NULL != obj_player) {
        k_destroy_object(obj_player);
        printf("destroy player\n");
    }
}

/* endregion */

/* region [game && room] */

static void destroy_player_from_room(void *unused) {

    if (k_is_key_pressed('1')) { destroy_player(players[0]); players[0] = NULL; }
    if (k_is_key_pressed('2')) { destroy_player(players[1]); players[1] = NULL; }
    if (k_is_key_pressed('3')) { destroy_player(players[2]); players[2] = NULL; }
}

static int create_room(struct k_room *room, void *params) {
    create_player(30, 30, 1);
    create_player(35, 35, 2);
    create_player(40, 40, 3);

    players[0] = create_player(130, 30, 3);
    players[1] = create_player(135, 35, 2);
    players[2] = create_player(140, 40, 1);

    create_player(230, 30, 2);
    create_player(235, 35, 3);
    create_player(240, 40, 1);

    k_room_add_step_callback(room, destroy_player_from_room, NULL);
    return 0;
}

static int init(void) {

    img = k_load_image("", "assets/tmp.png");
    (void)img;

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    room_config.room_name = "room";
    room_config.fn_init = create_room;

    struct k_room *room = k_create_room(&room_config, NULL);

    k_goto_room(room);
    return 0;
}

#if 0

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
