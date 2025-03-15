
#include "./demo_1.h"

enum my_player_state {
    player_idle,
    player_run,
};

struct my_player {
    float x;
    float y;

    float next_x;
    float next_y;

    int state;

    struct k_component *WASD;
    struct k_sprite_renderer *spr_rdr;
};

static void player_step_begin(struct k_object *object) {
    struct my_player *player = k_object_get_data(object);

    enum my_player_state state = player_idle;
    if (player->next_x < player->x) {
        k_sprite_renderer_flip_x(player->spr_rdr, 1);
        state = player_run;
    }
    else if (player->next_x > player->x) {
        k_sprite_renderer_flip_x(player->spr_rdr, 0);
        state = player_run;
    }

    switch (state) {
        case player_run: {
            if (state != player->state) {
                player->state = player_run;
                k_sprite_renderer_set_sprite(player->spr_rdr, k_sprite_find("player/run"));
            }
            break;
        }
        case player_idle: {
            if (player_idle != player->state) {
                 player->state = player_idle;
                k_sprite_renderer_set_sprite(player->spr_rdr, k_sprite_find("player/idle"));
            }
            break;
        }
    }

    player->x = player->next_x;
    player->y = player->next_y;
}

static void player_step(struct k_object *object) {
    struct my_player *player = k_object_get_data(object);

    float delta = k_get_step_delta();

    if (k_key_down('R'))
        k_sprite_renderer_adjust_rotation(player->spr_rdr, 180.0f * delta);

    if (k_key_down('F')) {

        if (k_key_pressed('X'))
            k_sprite_renderer_flip_x(player->spr_rdr, 1);
        if (k_key_pressed('Y'))
            k_sprite_renderer_flip_y(player->spr_rdr, 1);
    }

    if ( ! k_key_down(K_KEY_LEFT_SHIFT)) {
        if (k_key_down('X'))
            k_sprite_renderer_adjust_w(player->spr_rdr, 1);
        if (k_key_down('Y'))
            k_sprite_renderer_adjust_h(player->spr_rdr, 1);
    } else {
        if (k_key_down('X'))
            k_sprite_renderer_adjust_w(player->spr_rdr, -1);
        if (k_key_down('Y'))
            k_sprite_renderer_adjust_h(player->spr_rdr, -1);
    }

    if (k_key_down('='))
        k_sprite_renderer_add_speed(player->spr_rdr, 1.0f * delta);
    if (k_key_down('-'))
        k_sprite_renderer_add_speed(player->spr_rdr, -0.5f * delta);

    if (k_key_pressed('C'))
        k_sprite_renderer_clear_transforms(player->spr_rdr);
}

struct k_object *my_create_player(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct my_player));

    k_object_add_step_begin_callback(object, player_step_begin);

    struct my_player *player = k_object_get_data(object);
    player->x = x;
    player->y = y;
    player->next_x = x;
    player->next_y = y;
    player->state = player_idle;

    struct k_component_type *WASD = k_component_type_find("k/WASD");
    struct k_WASD_config WASD_config;
    WASD_config.key_up    = 'W';
    WASD_config.key_left  = 'A';
    WASD_config.key_down  = 'S';
    WASD_config.key_right = 'D';
    WASD_config.speed     = 150.0f;
    WASD_config.x         = &player->next_x;
    WASD_config.y         = &player->next_y;
    player->WASD = k_object_add_component(object, WASD, &WASD_config);

    struct k_sprite_renderer_config renderer_config;
    renderer_config.sprite  = k_sprite_find("player/idle");
    renderer_config.z_index = 0;
    renderer_config.x       = &player->x;
    renderer_config.y       = &player->y;
    player->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);

    return object;
}

