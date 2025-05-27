
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "k_game.h"

#include "object/fighter/player/yx_obj_player.h"

/* region [movement_sm] */

static void yx__obj_player_on_idle_state_enter(struct k_object *object);
static void yx__obj_player_on_idle_state_step(struct k_object *object);
static struct yx_state_machine_state STATE_IDLE = {
    yx__obj_player_on_idle_state_enter,
    yx__obj_player_on_idle_state_step,
    NULL
};

static void yx__obj_player_on_running_state_enter(struct k_object *object);
static void yx__obj_player_on_running_state_step(struct k_object *object);
static struct yx_state_machine_state STATE_RUNNING = {
    yx__obj_player_on_running_state_enter,
    yx__obj_player_on_running_state_step,
    NULL
};

/* region [idle] */

static void yx__obj_player_on_idle_state_enter(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);
    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_idle);
    player->vx_movement = 0.0f;
    player->vy_movement = 0.0f;
}

static void yx__obj_player_on_idle_state_step(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    if (k_key_held(player->key_up)
     || k_key_held(player->key_left)
     || k_key_held(player->key_down)
     || k_key_held(player->key_right)
    ) {
        yx_state_machine_change_state(&player->movement_sm, &STATE_RUNNING);
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

    float vx = 0.0f;
    float vy = 0.0f;

    if (k_key_held(player->key_up))    { vy = -player->speed; }
    if (k_key_held(player->key_left))  { vx = -player->speed; }
    if (k_key_held(player->key_down))  { vy =  player->speed; }
    if (k_key_held(player->key_right)) { vx =  player->speed; }

#define inv_sqrt_2 0.70710678f
    if (vx != 0.0f && vy != 0.0f) {
        vx *= inv_sqrt_2;
        vy *= inv_sqrt_2;
    }

    player->vx_movement = vx;
    player->vy_movement = vy;

    if (0.0f == vx && 0.0f == vy) {
        yx_state_machine_change_state(&player->movement_sm, &STATE_IDLE);
    }
}

/* endregion */

/* endregion */

static void yx__obj_player_resolve_movement(struct yx_obj_player *player) {

    float dt = k_time_get_step_delta();

    float vx = 0.0f;
    float vy = 0.0f;

    {
        /* 处理击退 */

        float knockback_damping = 0.85f;
        if (fabsf(player->vx_knockback) >= 0.01f) {
            vx += player->vx_knockback;
            player->vx_knockback *= knockback_damping;
        } else {
            player->vx_knockback = 0.0f;
        }
        if (fabsf(player->vy_knockback) >= 0.01f) {
            vy += player->vy_knockback;
            player->vy_knockback *= knockback_damping;
        } else {
            player->vy_knockback = 0.0f;
        }
    }

    {
        /* 按键操控角色移动 */

        vx += player->vx_movement;
        vy += player->vy_movement;
    }

    {
        /* 限制在房间内 */

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

    player->x = player->x + vx * dt; /* 移动对象 */
    player->y = player->y + vy * dt;
    k_sprite_renderer_set_z_layer(player->spr_rdr, (int)player->y);
    {
        float mouse_x = k_mouse_x();
        if (mouse_x < player->x) {
            k_sprite_renderer_flip_x(player->spr_rdr, 0);
        } else if (mouse_x > player->x) {
            k_sprite_renderer_flip_x(player->spr_rdr, 1);
        }
    }

    k_position_set_local_position(player->position, player->x, player->y);
}

static void yx__obj_player_on_step_update_weapon(struct yx_obj_player *player) {

    if (player->ammo <= player->ammo_max) {
        player->ammo_timer += k_time_get_step_delta();
        if (player->ammo_timer >= 3.0f) {
            player->ammo_timer -= 3.0f;
            player->ammo += 1;
        }
    }

    yx_obj_player_weapon_set_position(player->weapon, player->x, player->y, (int)player->y + 1);

    float mouse_x = k_mouse_x();
    float mouse_y = k_mouse_y();
    yx_obj_player_weapon_aim_at(player->weapon, mouse_x, mouse_y);

    if (k_mouse_button_down(K_BUTTON_LEFT)) {
        yx_obj_player_weapon_on_key_down(player->weapon, &player->ammo);
    }
    if (k_mouse_button_held(K_BUTTON_LEFT)) {
        yx_obj_player_weapon_on_key_held(player->weapon, &player->ammo);
    }
    if (k_mouse_button_up(K_BUTTON_LEFT)) {
        yx_obj_player_weapon_on_key_up(player->weapon, &player->ammo);
    }
}

static void yx__obj_player_on_step_resolve_movement(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    yx_state_machine_tick(&player->movement_sm);
    yx__obj_player_on_step_check_hit_bullet(player);
    yx__obj_player_resolve_movement(player);
    yx__obj_player_on_step_update_weapon(player);
}

int yx__obj_player_on_create_init_movement(struct yx_obj_player *player) {

    yx_state_machine_init(player->object, &player->movement_sm);
    yx_state_machine_change_state(&player->movement_sm, &STATE_IDLE);

    if (NULL == k_object_add_step_callback(player->object, yx__obj_player_on_step_resolve_movement))
        return -1;

    struct k_camera_target *target = k_camera_add_follow_target(player->object, &player->x, &player->y);
    if (NULL == target)
        return -1;

    k_camera_set_primary_target(target);
    k_camera_set_target_weight(target, 4.0f);

    return 0;
}
