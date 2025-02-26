#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"
#include "k_seq_step.h"

/* region [img] */

static struct k_image *img;

/* endregion */

/* region [movement_component] */

struct comp_movement {
    struct k_component_callback *alarm_3;
    struct k_component_callback *alarm_6;
    float x;
    float y;
};

void comp_create(struct k_component *component, void *params) {

}

void comp_alarm_3(struct k_component *comp, int timeout_diff) {

}

void comp_alarm_6(struct k_component *comp, int timeout_diff) {

}

void comp_step(struct k_component *comp) {

}

void comp_draw(struct k_component *comp) {

}

void define_movement_component(void) {

    struct k_component_type_config config;
    config.component_type_name = "my_component";
    config.data_size = sizeof(struct comp_movement);
    config.fn_create = comp_create;
    config.fn_destroy = NULL;
    struct k_component_callback_config callbacks[] = {
        { offsetof(struct comp_movement, alarm_3), K_EVENT_ALARM, .alarm_callback = { comp_alarm_3, 6000 } },
        { offsetof(struct comp_movement, alarm_6), K_EVENT_ALARM, .alarm_callback = { comp_alarm_6, 3000 } },
        { SIZE_MAX,                                K_EVENT_STEP,  .step_callback  = { comp_step          } },
        { SIZE_MAX,                                K_EVENT_DRAW,  .draw_callback  = { comp_draw, 3       } },
    };
    config.callbacks = callbacks;
    config.callbacks_num = k_array_len(callbacks);

    struct k_component_type *component_type = k_define_component_type(&config);
    (void)component_type;
}

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

static int create_room(struct k_room *room, void *params) {
    create_player(60, 60, 1);
    return 0;
}

static int init(void) {

    img = k_load_image("", "assets/tmp.png");
    (void)img;

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    room_config.room_name = "room";
    room_config.fn_create = create_room;

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
