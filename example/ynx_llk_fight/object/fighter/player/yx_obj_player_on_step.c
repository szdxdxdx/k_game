
#include <stdlib.h>

#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_collision_group.h"
#include "object/particle/yx_obj_text_particle.h"
#include "object/particle/yx_obj_particle_on_hit.h"
#include "object/particle/yx_ui_banner.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "utils/yx_math.h"
#include "object/weapon/apple/yx_obj_weapon_apple.h"

/* region [move] */

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

static void yx__obj_player_on_dead_state_enter(struct k_object *object);
static void yx__obj_player_on_dead_state_step(struct k_object *object);
static void yx__obj_player_on_dead_state_leave(struct k_object *object);
static struct yx_state_machine_state STATE_DEAD = {
    yx__obj_player_on_dead_state_enter,
    yx__obj_player_on_dead_state_step,
    yx__obj_player_on_dead_state_leave,
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

/* region [dead] */

static void yx__obj_player_on_dead_state_enter(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_dead);
    k_sprite_renderer_set_loop_count(player->spr_rdr, 1);

    yx_obj_player_weapon_destroy(player->weapon);
    player->weapon = NULL;

    player->vx_movement = 0.0f;
    player->vy_movement = 0.0f;

    player->hp = 0;
    player->ammo = 0;
}

static void yx__obj_player_on_dead_state_step(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    int old_hp = (int)player->hp;
    player->hp += (player->hp_max / 6.0f) * k_time_get_step_delta();
    int new_hp = (int)player->hp;

    if (player->hp_max <= player->hp) {
        player->hp = player->hp_max;
        player->ammo = player->ammo_max;
        yx_state_machine_change_state(&player->movement_sm, &STATE_IDLE);
    }
    else {
        player->ammo = (int)(player->hp / player->hp_max * (float)player->ammo_max);
    }

    if (new_hp % 3 == 0 && old_hp % 3 != 0) {
        struct yx_float_vec2 v_text = yx_float_vec2_new(0, -yx_rand(65.0f, 80.0f));
        struct yx_obj_particle_text_on_hit_config config;
        config.x = player->x + yx_rand(-20.0f, 20.0f);
        config.y = player->y + yx_rand(-4.0f, 4.0f);
        config.vx = v_text.x;
        config.vy = v_text.y;
        config.color = 0x0bf00bff;
        yx_obj_particle_text_on_hit_create(&config, "+HP");
    }
}

static void yx__obj_player_on_dead_state_leave(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    struct yx_obj_particle_on_hit *fx = yx_obj_particle_on_hit_create(player->x, player->y);
    k_sprite_renderer_scale(fx->spr_rdr, 3.2f);
    k_sprite_renderer_set_duration(fx->spr_rdr, 0.65f);

    k_camera_shake(8.0f, 0.5f);

    player->weapon = yx_obj_player_weapon_apple_create();
}

/* endregion */

/* endregion */

/* region [collision] */

static void yx__obj_rival_create_text_particle_on_hit(struct yx_obj_player *player, struct yx_bullet_on_hit_result *hit_result) {

    struct yx_float_vec2 v_text = yx_float_vec2_new(hit_result->vx_knockback, hit_result->vy_knockback);
    v_text = yx_float_vec2_perp_right(v_text);
    v_text = (rand() % 2) ? v_text : yx_float_vec2_neg(v_text);
    v_text = yx_float_vec2_normalize(v_text);
    v_text = yx_float_vec2_scale(v_text, yx_rand(40.0f, 60.0f));

    struct yx_obj_particle_text_on_hit_config config;
    config.x = player->x;
    config.y = player->y;
    config.vx = v_text.x;
    config.vy = v_text.y;
    config.color = 0xf00b0bff;
    yx_obj_particle_text_on_hit_create(&config, "-%d", (int)hit_result->damage);
}

void yx__obj_player_on_step_check_hit_bullet(struct yx_obj_player *player) {

    struct k_collision_box *bullet_box = k_collision_check_box(YX_CONFIG_COLLISION_GROUP_RIVAL_BULLET, player->hp_collision_box);
    if (NULL == bullet_box)
        return;

    struct yx_obj_player_bullet *bullet = k_object_get_data(k_collision_box_get_object(bullet_box));
    struct yx_bullet_on_hit_result hit_result;
    yx_obj_player_bullet_on_hit(bullet, &hit_result);

    yx_obj_particle_on_hit_create(player->x, player->y);

    player->vx_knockback += hit_result.vx_knockback;
    player->vy_knockback += hit_result.vy_knockback;

    if (&STATE_DEAD != yx_state_machine_get_current_state(&player->movement_sm)) {
        if (0.0f < player->hp) {
            player->hp -= hit_result.damage;
            yx__obj_rival_create_text_particle_on_hit(player, &hit_result);

            if (player->hp <= 0.0f) {
                player->hp = 0.0f;
                yx_ui_banner_show_YOU_DIED();
                yx_state_machine_change_state(&player->movement_sm, &STATE_DEAD);
            }
        }
    }
}

int yx__obj_player_on_create_init_collision(struct yx_obj_player *player) {

    struct k_collision_rect_config config;
    config.x = &player->x;
    config.y = &player->y;
    config.group_id = YX_CONFIG_COLLISION_GROUP_PLAYER_HP;
    config.offset_x1 = -14.0f;
    config.offset_y1 = -20.0f;
    config.offset_x2 =  14.0f;
    config.offset_y2 =  10.0f;
    player->hp_collision_box = k_object_add_collision_rect(player->object, &config);
    if (NULL == player->hp_collision_box)
        return -1;

    return 0;
}

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

    if (NULL == player->weapon)
        return;

    if (player->ammo < player->ammo_max) {
        player->ammo_timer += k_time_get_step_delta();
        if (player->ammo_timer >= 1.2f) {
            player->ammo_timer -= 1.2f;
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

/* endregion */
