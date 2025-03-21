#include "../_internal.h"

static void player_step_set_state(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    enum yx_obj_player_state state = player_idle;
    if (player->next_x < player->x) {
        state = player_run;
        player->face = -1;
    }
    else if (player->next_x > player->x) {
        state = player_run;
        player->face = 1;
    }
    else if (player->next_y != player->y) {
        state = player_run;
    }

    switch (state) {
        case player_run: {

            switch (player->state) {
                case player_run:
                    if (player->face == 1)
                        k_sprite_renderer_flip_x(player->spr_rdr, 1);
                    else
                        k_sprite_renderer_flip_x(player->spr_rdr, 0);
                    break;
                case player_idle:
                    player->state = player_run;
                    k_sprite_renderer_set_sprite(player->spr_rdr, yx_spr_ynx_run);
                    if (player->face == 1)
                        k_sprite_renderer_flip_x(player->spr_rdr, 1);
                    break;
            }
            break;
        }
        case player_idle: {

            switch (player->state) {
                case player_run:player->state = player_idle;
                    k_sprite_renderer_set_sprite(player->spr_rdr, yx_spr_ynx_idle);

                    if (player->face == 1)
                        k_sprite_renderer_flip_x(player->spr_rdr, 1);
                case player_idle:
                    break;
            }
            break;
        }
    }

    player->x = player->next_x;
    player->y = player->next_y;
}

static void player_step_shoot(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    if (k_key_pressed('Q')) {
        yx_player_bullet_create(
        player->x + (float)player->face * 40,
        player->y - 10.f,
        player->face);
    }
}

static void player_step(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    float delta = k_get_step_delta();

    /* 旋转 */
    if (k_key_down('R'))
        k_sprite_renderer_rotate(player->spr_rdr, 180.0f * delta + k_sprite_renderer_get_rotation(player->spr_rdr));

    /* 翻转 */
    if (k_key_down('F')) {
        if (k_key_pressed('X'))
            k_sprite_renderer_flip_x(player->spr_rdr, 1);
        if (k_key_pressed('Y'))
            k_sprite_renderer_flip_y(player->spr_rdr, 1);
        if (k_key_pressed('C'))
            k_sprite_renderer_flip_y(player->spr_rdr, 0);
    }

    /* 放缩 */
    if (k_key_down('Z')) {
        if (k_key_down('X'))
            k_sprite_renderer_set_w(player->spr_rdr, 6 + k_sprite_renderer_get_w(player->spr_rdr));
        if (k_key_down('Y'))
            k_sprite_renderer_set_h(player->spr_rdr, 6 + k_sprite_renderer_get_h(player->spr_rdr));
    }

    /* 加速减速 */
    if (k_key_down('='))
        k_sprite_renderer_set_speed(player->spr_rdr, 1.0f * delta + k_sprite_renderer_get_speed(player->spr_rdr));
    if (k_key_down('-'))
        k_sprite_renderer_set_speed(player->spr_rdr, -0.5f * delta + k_sprite_renderer_get_speed(player->spr_rdr));

    if (k_key_pressed('B')) {
        if (k_key_held(K_KEY_LEFT_SHIFT))
            k_sprite_renderer_set_debug(player->spr_rdr, 0);
        else
            k_sprite_renderer_set_debug(player->spr_rdr, 1);
    }

    if (k_key_pressed('H')) {
        k_sprite_renderer_set_sprite(player->spr_rdr, NULL);
    }
    if (k_key_pressed('G')) {
        k_sprite_renderer_set_sprite(player->spr_rdr, yx_spr_ynx_run);
    }
}

struct k_object *yx_player_create(float x, float y) {
    struct k_room *room = k_get_current_room();

    struct k_object *object = k_object_create(sizeof(struct yx_obj_player));

    k_object_add_step_begin_callback(object, player_step_set_state);
    k_object_add_step_callback(object, player_step);
    k_object_add_step_callback(object, player_step_shoot);

    struct yx_obj_player *player = k_object_get_data(object);
    player->x      = x;
    player->y      = y;
    player->next_x = x;
    player->next_y = y;
    player->state  = player_idle;
    player->face   = -1;

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
    renderer_config.sprite  = yx_spr_ynx_idle;
    renderer_config.z_index = 0;
    renderer_config.x       = &player->x;
    renderer_config.y       = &player->y;
    player->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);

    return object;
}
