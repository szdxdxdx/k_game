#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

/* region [img] */

static struct k_image *img;

/* endregion */

/* region [movement_component] */

struct my_movement_component_config {
    int up_key;
    int down_key;
    int left_key;
    int right_key;
    float speed;
};

struct my_movement_component {
    struct k_component_callback *alarm_6;
    float x;
    float y;
    int up_key;
    int down_key;
    int left_key;
    int right_key;
    float speed;
};

void comp_alarm_3(struct k_component *comp, int timeout_diff) {

    printf("3\n");
    if (k_is_key_down('B')) {
        printf("remove alarm callback 6\n");
        struct my_movement_component *movement = k_component_get_data(comp);
        k_component_del_callback(movement->alarm_6);
    }
}

void comp_alarm_6(struct k_component *comp, int timeout_diff) {

    struct my_movement_component *movement = k_component_get_data(comp);
    movement->alarm_6 = NULL;
    printf("6\n");
}

void comp_step(struct k_component *comp) {
    struct my_movement_component *movement = k_component_get_data(comp);
    float delta_ms = k_get_step_delta();

    if (k_is_key_down(movement->up_key))
        movement->y -= movement->speed * delta_ms;
    if (k_is_key_down(movement->left_key))
        movement->x -= movement->speed * delta_ms;
    if (k_is_key_down(movement->right_key))
        movement->x += movement->speed * delta_ms;
    if (k_is_key_down(movement->down_key))
        movement->y += movement->speed * delta_ms;
}

int comp_create(struct k_component *component, void *params) {
    struct my_movement_component *movement = k_component_get_data(component);
    struct my_movement_component_config *config = params;
    movement->x = 0;
    movement->y = 0;

    movement->up_key = config->up_key;
    movement->down_key = config->down_key;
    movement->left_key = config->left_key;
    movement->right_key = config->right_key;
    movement->speed = config->speed;

    k_component_add_alarm_callback(component, comp_alarm_3, 3000);
    movement->alarm_6 = k_component_add_alarm_callback(component, comp_alarm_6, 6000);
    k_component_add_step_callback(component, comp_step);

    return 0;
}

void define_movement_component(void) {

    struct k_component_type_config config;
    config.type_name = "my_movement";
    config.data_size = sizeof(struct my_movement_component);
    config.fn_create = comp_create;
    config.fn_destroy = NULL;

    struct k_component_type *component_type = k_define_component_type(&config);
    (void)component_type;
}

/* endregion */

/* region [obj_player] */

struct obj_player {
    struct k_component *movement;
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

    struct my_movement_component *movement = k_component_get_data(player->movement);
    k_draw_image(img, &rect, (int)movement->x, (int)movement->y);

    uint64_t current_ms = k_get_step_timestamp();
    if (player->spr_timer <= current_ms) {
        player->spr_timer += 150;

        player->current_frame += 1;
        player->current_frame %= 4;
    }
}

static struct k_object *create_player(int up_key, int down_key, int left_key, int right_key, float speed) {

    struct k_object *object = k_create_object(sizeof(struct obj_player));
    if (NULL == object)
        return NULL;

    struct obj_player *player = k_object_get_data(object);
    player->spr_timer = 0;
    player->current_frame = 0;

    struct my_movement_component_config movement_config;
    movement_config.up_key = up_key;
    movement_config.down_key = down_key;
    movement_config.left_key = left_key;
    movement_config.right_key = right_key;
    movement_config.speed = speed;
    player->movement = k_object_add_component(object, k_get_component_type_by_name("my_movement"), &movement_config);

    k_object_add_draw_callback(object, object_draw, 0);

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

static int create_room(struct k_room *room, void *params) {
    create_player('W', 'S', 'A', 'D', 100.0f);
    create_player('I', 'K', 'J', 'L', 150.0f);
    return 0;
}

static int init(void) {

    img = k_load_image("", "assets/tmp.png");
    (void)img;

    define_movement_component();

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
