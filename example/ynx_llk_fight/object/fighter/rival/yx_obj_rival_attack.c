#include <assert.h>

#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_arena_blackboard.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "object/alert_marker/yx_obj_alert_marker.h"
#include "utils/yx_math.h"
#include "config/yx_config_collision_group.h"
#include "sprite/yx_spr.h"
#include "config/yx_config_z_index.h"

/* region [bullet_apple] */



static void yx__obj_rival_bullet_apple_on_step(struct k_object *object) {
    struct yx_obj_rival_bullet_apple *bullet_apple = k_object_get_data(object);

    float dt = k_time_get_step_delta();

    bullet_apple->x += bullet_apple->vx * dt;
    bullet_apple->y += bullet_apple->vy * dt;

    float padding = -30.0f;
    struct k_float_rect room_rect = {
        .x = padding,
        .y = padding,
        .w = k_room_get_w() - padding,
        .h = k_room_get_h() - padding
    };
    if ( ! yx_point_in_rect(bullet_apple->x, bullet_apple->y, &room_rect)) {
        k_object_destroy(object);
        return;
    }

    float angle = k_sprite_renderer_get_rotation(bullet_apple->spr_rdr);
    angle += dt * bullet_apple->rotation_speed;
    k_sprite_renderer_rotate(bullet_apple->spr_rdr, angle);
}

static struct yx_obj_rival_bullet_apple *yx__obj_rival_bullet_apple_create(struct yx_obj_rival *rival) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_rival_bullet_apple));
    if (NULL == object)
        goto err;

    struct yx_obj_rival_bullet_apple *bullet_apple = k_object_get_data(object);

    {
        struct k_collision_rect_config config;
        config.x = &bullet_apple->x;
        config.y = &bullet_apple->y;
        config.group_id = YX_CONFIG_COLLISION_GROUP_RIVAL_BULLET_APPLE;
        config.offset_x1 = -16.0f;
        config.offset_y1 = -16.0f;
        config.offset_x2 = 16.0f;
        config.offset_y2 = 16.0f;
        bullet_apple->hit_box = k_object_add_collision_rect(object, &config);
        if (NULL == bullet_apple->hit_box)
            goto err;
    }

    bullet_apple->x = rival->x;
    bullet_apple->y = rival->y;

    bullet_apple->rotation_speed = yx_rand(300.0f, 800.0f);

    struct yx_obj_player *player = rival->blackboard->player;

    float sin_angle;
    float cos_angle;
    yx_calc_vector_direction(rival->x, rival->y, player->x, player->y, &cos_angle, &sin_angle);

    float speed = yx_rand(300.0f, 380.0f);
    bullet_apple->vx = cos_angle * speed;
    bullet_apple->vy = sin_angle * speed;

    {
        struct k_sprite_renderer_config config;
        config.x       = &bullet_apple->x;
        config.y       = &bullet_apple->y;
        config.sprite  = yx_spr_bullet_apple;
        config.z_group = YX_CONFIG_Z_GROUP_BULLET;
        config.z_layer = 0;
        bullet_apple->spr_rdr = k_object_add_sprite_renderer(object, &config);
        if (NULL == bullet_apple->spr_rdr)
            goto err;

        float angle = 0.0f;// k_sprite_renderer_get_rotation(weapon_apple->spr_rdr);
        k_sprite_renderer_rotate(bullet_apple->spr_rdr, angle);
    }

    if (NULL == k_object_add_step_callback(object, yx__obj_rival_bullet_apple_on_step))
        goto err;

    return bullet_apple;

err:
    k_object_destroy(object);
    k_log_error("failed to create object: yx_obj_rival_bullet_apple");
    return NULL;
}

/* endregion */

/* region [attack_sm] */

static void yx__obj_rival_on_state_patrol_enter(struct k_object *object);
static void yx__obj_rival_on_state_patrol_update(struct k_object *object);
static struct yx_state_machine_state YX_STATE_PATROL = {
    .on_enter  = yx__obj_rival_on_state_patrol_enter,
    .on_update = yx__obj_rival_on_state_patrol_update,
    .on_leave  = NULL,
};

static void yx__obj_rival_on_state_attack_enter(struct k_object *object);
static void yx__obj_rival_on_state_attack_update(struct k_object *object);
static struct yx_state_machine_state YX_STATE_ATTACK = {
    .on_enter  = yx__obj_rival_on_state_attack_enter,
    .on_update = yx__obj_rival_on_state_attack_update,
    .on_leave  = NULL,
};

/* region [patrol] */

static void yx__obj_rival_on_state_patrol_enter(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    rival->attack_state = YX_OBJ_RIVAL_STATE_PATROL;
}

static void yx__obj_rival_on_state_patrol_update(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    struct yx_obj_player *player = rival->blackboard->player;

    struct yx_float_vec2 dir = yx_float_vec2_new(player->x - rival->x, player->y - rival->y);

    if (yx_float_vec2_length(dir) <= YX__OBJ_RIVAL_AGGRO_RADIUS) { /* 与玩家距离太近，切换成攻击状态 */
        yx_state_machine_change_state(&rival->attack_sm, &YX_STATE_ATTACK);
    }
}

/* endregion */

/* region [attack] */

static void yx__obj_rival_on_state_attack_enter(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    rival->attack_state = YX_OBJ_RIVAL_STATE_ATTACK;

    yx_obj_alert_marker_create(rival->position, -30, -16); /* 感叹号气泡 */

    rival->attack_timer = yx_rand(0.8f, 2.5f);
}

static void yx__obj_rival_on_state_attack_update(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    struct yx_obj_player *player = rival->blackboard->player;

    struct yx_float_vec2 dir = yx_float_vec2_new(player->x - rival->x, player->y - rival->y);

    if (yx_float_vec2_length(dir) >= YX__OBJ_RIVAL_LOSE_TARGET_RADIUS) { /* 与玩家距离太远，切换成巡逻状态 */
        yx_state_machine_change_state(&rival->attack_sm, &YX_STATE_PATROL);
        return;
    }

    float dt = k_time_get_step_delta();
    rival->attack_timer -= dt;
    if (rival->attack_timer <= 0.0f) {
        rival->attack_timer = yx_rand(0.4f, 2.5f);

        yx__obj_rival_bullet_apple_create(rival);
    }
}

/* endregion */

/* endregion */

static void yx__obj_rival_on_step_attack(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    yx_state_machine_tick(&rival->attack_sm);
}

int yx__obj_rival_on_create_add_attack(struct yx_obj_rival *rival) {

    if (NULL == k_object_add_step_callback(rival->object, yx__obj_rival_on_step_attack))
        return -1;

    yx_state_machine_init(&rival->attack_sm, rival->object);
    yx_state_machine_change_state(&rival->attack_sm, &YX_STATE_PATROL);

    return 0;
}
