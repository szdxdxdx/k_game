
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "k_game.h"

#include "object/fighter/player/yx_obj_player.h"

static void yx__obj_player_on_step_change_face(struct yx_obj_player *player) {

    float mouse_x = k_mouse_x();

    if (mouse_x < player->x) {
        k_sprite_renderer_flip_x(player->spr_rdr, 0);
    }
    else if (mouse_x > player->x) {
        k_sprite_renderer_flip_x(player->spr_rdr, 1);
    }
}

static void yx__obj_player_on_step_update_weapon(struct yx_obj_player *player) {

    yx_obj_weapon_set_position(player->weapon, player->x, player->y, (int)player->y + 1);

    float mouse_x = k_mouse_x();
    float mouse_y = k_mouse_y();
    yx_obj_weapon_aim_at(player->weapon, mouse_x, mouse_y);

    if (k_mouse_button_down(K_BUTTON_LEFT)) {
        yx_obj_weapon_on_key_down(player->weapon);
    }
    if (k_mouse_button_held(K_BUTTON_LEFT)) {
        yx_obj_weapon_on_key_held(player->weapon);
    }
    if (k_mouse_button_up(K_BUTTON_LEFT)) {
        yx_obj_weapon_on_key_up(player->weapon);
    }
}

/* region [state] */

static void yx__obj_player_on_idle_state_enter(struct k_object *object);
static void yx__obj_player_on_idle_state_step(struct k_object *object);
static struct k_state_machine_state STATE_IDLE = {
    yx__obj_player_on_idle_state_enter,
    yx__obj_player_on_idle_state_step,
    NULL
};

static void yx__obj_player_on_running_state_enter(struct k_object *object);
static void yx__obj_player_on_running_state_step(struct k_object *object);
static struct k_state_machine_state STATE_RUNNING = {
    yx__obj_player_on_running_state_enter,
    yx__obj_player_on_running_state_step,
    NULL
};

/* region [idle] */

static void yx__obj_player_on_idle_state_enter(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);
    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_idle);
}

static void yx__obj_player_on_idle_state_step(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    yx__obj_player_on_step_change_face(player);
    yx__obj_player_on_step_update_weapon(player);

    if (k_key_held(player->key_up)
     || k_key_held(player->key_left)
     || k_key_held(player->key_down)
     || k_key_held(player->key_right)
    ) {
        k_state_machine_change_state(player->movement_sm, &STATE_RUNNING);
    }
}

/* endregion */

/* region [running] */

static void yx__obj_player_on_running_state_enter(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);
    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_run);
}

static void yx__obj_player_on_running_state_step(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    float dt = k_time_get_step_delta();
    float dx = 0.0f;
    float dy = 0.0f;

    if (k_key_held(player->key_up))    { dy = -player->speed * dt; }
    if (k_key_held(player->key_left))  { dx = -player->speed * dt; }
    if (k_key_held(player->key_down))  { dy =  player->speed * dt; }
    if (k_key_held(player->key_right)) { dx =  player->speed * dt; }

#define inv_sqrt_2 0.70710678f
    if (dx != 0.0f && dy != 0.0f) {
        dx *= inv_sqrt_2;
        dy *= inv_sqrt_2;
    }

    float old_y = player->y;

    player->x += dx;
    player->y += dy;

    {
        float room_padding = 16.0f;
        float x_min = room_padding;
        float x_max = k_room_get_w() - room_padding;
        float y_min = room_padding;
        float y_max = k_room_get_h() - room_padding;
        if (player->x < x_min) player->x = x_min;
        if (player->x > x_max) player->x = x_max;
        if (player->y < y_min) player->y = y_min;
        if (player->y > y_max) player->y = y_max;
    }

    if (old_y != player->y) {
        k_sprite_renderer_set_z_layer(player->spr_rdr, (int)player->y);
    }

    yx__obj_player_on_step_change_face(player);
    yx__obj_player_on_step_update_weapon(player);

    if (0.0f == dx && 0.0f == dy) {
        k_state_machine_change_state(player->movement_sm, &STATE_IDLE);
    }
}

/* endregion */

/* endregion */

int yx__obj_player_on_create_add_movement(struct yx_obj_player *player) {

    {
        player->movement_sm = k_object_add_state_machine(player->object);
        if (NULL == player->movement_sm)
            return -1;

        k_state_machine_change_state(player->movement_sm, &STATE_IDLE);
    }

    {
        struct k_camera_target *target = k_camera_add_follow_target(player->object, &player->x, &player->y);
        if (NULL == target)
            return -1;

        k_camera_set_primary_target(target);
        k_camera_set_target_weight(target, 4.0f);
    }

    return 0;
}
